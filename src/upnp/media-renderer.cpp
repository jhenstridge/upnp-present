#include "media-renderer.h"

#include <libgupnp-av/gupnp-last-change-parser.h>
#include <QDebug>

#include <stdexcept>

namespace {
const char CONNECTION_MANAGER[] = "urn:schemas-upnp-org:service:ConnectionManager";
const char AV_TRANSPORT[] = "urn:schemas-upnp-org:service:AVTransport";
const char RENDERING_CONTROL[] = "urn:schemas-upnp-org:service:RenderingControl";

gobj_ptr<GUPnPLastChangeParser> lc_parser(gupnp_last_change_parser_new());
}

namespace upnp {

MediaRenderer::MediaRenderer(const gobj_ptr<GUPnPDeviceProxy> &device, QObject *parent)
    : QObject(parent), device(device) {
    GUPnPDeviceInfo *info = GUPNP_DEVICE_INFO(device.get());
    connection_manager.reset(GUPNP_SERVICE_PROXY(gupnp_device_info_get_service(info, CONNECTION_MANAGER)));
    if (!connection_manager) {
        throw std::runtime_error("Device does not implement ConnectionManager");
    }

    av_transport.reset(GUPNP_SERVICE_PROXY(gupnp_device_info_get_service(info, AV_TRANSPORT)));
    if (!av_transport) {
        throw std::runtime_error("Device does not implement AVTransport");
    }

    rendering_control.reset(GUPNP_SERVICE_PROXY(gupnp_device_info_get_service(info, RENDERING_CONTROL)));
    if (!connection_manager) {
        throw std::runtime_error("Device does not implement RenderingControl");
    }

    // Subscribe to change notifications
    gupnp_service_proxy_add_notify(
        av_transport.get(), "LastChange", G_TYPE_STRING,
        &MediaRenderer::av_transport_last_change_cb, this);
    gupnp_service_proxy_set_subscribed(av_transport.get(), true);
    gupnp_service_proxy_add_notify(
        rendering_control.get(), "LastChange", G_TYPE_STRING,
        &MediaRenderer::rendering_control_last_change_cb, this);
    gupnp_service_proxy_set_subscribed(rendering_control.get(), true);

    // And get current values
    get_protocol_info_action = gupnp_service_proxy_begin_action(
        connection_manager.get(), "GetProtocolInfo",
        &MediaRenderer::get_protocol_info_cb, this,
        nullptr);
    get_transport_info_action = gupnp_service_proxy_begin_action(
        av_transport.get(), "GetTransportInfo",
        &MediaRenderer::get_transport_info_cb, this,
        "InstanceID", G_TYPE_UINT, 0,
        nullptr);
    get_media_info_action = gupnp_service_proxy_begin_action(
        av_transport.get(), "GetMediaInfo",
        &MediaRenderer::get_media_info_cb, this,
        "InstanceID", G_TYPE_UINT, 0,
        nullptr);
    get_volume_action = gupnp_service_proxy_begin_action(
        rendering_control.get(), "GetVolume",
        &MediaRenderer::get_volume_cb, this,
        "InstanceID", G_TYPE_UINT, 0,
        "Channel", G_TYPE_STRING, "Master",
        nullptr);
}

MediaRenderer::~MediaRenderer() {
    if (get_protocol_info_action) {
        gupnp_service_proxy_cancel_action(
            connection_manager.get(), get_protocol_info_action);
    }
    if (get_transport_info_action) {
        gupnp_service_proxy_cancel_action(
            av_transport.get(), get_transport_info_action);
    }
    if (get_media_info_action) {
        gupnp_service_proxy_cancel_action(
            av_transport.get(), get_media_info_action);
    }
    if (get_volume_action) {
        gupnp_service_proxy_cancel_action(
            rendering_control.get(), get_volume_action);
    }
    if (av_transport) {
        gupnp_service_proxy_remove_notify(
            av_transport.get(), "LastChange",
            &MediaRenderer::av_transport_last_change_cb, this);
    }
    if (rendering_control) {
        gupnp_service_proxy_remove_notify(
            rendering_control.get(), "LastChange",
            &MediaRenderer::rendering_control_last_change_cb, this);
    }
}

void MediaRenderer::av_transport_last_change_cb(GUPnPServiceProxy *proxy, const char *variable, GValue *value, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);

    const char *last_change_xml = g_value_get_string(value);
    GError *error = nullptr;
    char *transport_state = nullptr;
    char *duration = nullptr;
    if (gupnp_last_change_parser_parse_last_change(
            lc_parser.get(), 0, last_change_xml, &error,
            "TransportState", G_TYPE_STRING, &transport_state,
            "CurrentMediaDuration", G_TYPE_STRING, &duration,
            nullptr)) {
        if (transport_state) {
            mr->transport_state = transport_state;
            g_free(transport_state);
            Q_EMIT mr->transportStateChanged();
        }
        if (duration) {
            mr->media_duration = duration;
            g_free(duration);
            Q_EMIT mr->mediaDurationChanged();
        }
    } else if (error) {
        qWarning() << "Failed to parse AVTransport LastChanged notification:" << error->message;
        g_error_free(error);
    }
}

void MediaRenderer::rendering_control_last_change_cb(GUPnPServiceProxy *proxy, const char *variable, GValue *value, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);

    const char *last_change_xml = g_value_get_string(value);
    GError *error = nullptr;
    uint volume;
    if (gupnp_last_change_parser_parse_last_change(
            lc_parser.get(), 0, last_change_xml, &error,
            "Volume", G_TYPE_UINT, &volume,
            nullptr)) {
        mr->volume_ = volume;
        Q_EMIT mr->transportStateChanged();
    } else if (error) {
        qWarning() << "Failed to parse RenderingControl LastChanged notification:" << error->message;
        g_error_free(error);
    }
}

void MediaRenderer::get_protocol_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);
    mr->get_protocol_info_action = nullptr;

    GError *error = nullptr;
    char *sink = nullptr;
    if (!gupnp_service_proxy_end_action(proxy, action, &error,
                                        "Sink", G_TYPE_STRING, &sink,
                                        nullptr)) {
        qWarning() << "GetProtocolInfo for" << mr->udn() << "failed:" << error->message;
        g_error_free(error);
        return;
    }

    QStringList sink_info = QString(sink).split(',', QString::SkipEmptyParts);
    if (mr->protocol_info != sink_info) {
        mr->protocol_info = sink_info;
        Q_EMIT mr->protocolInfoChanged();
    }
    g_free(sink);
}

void MediaRenderer::get_transport_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);
    mr->get_transport_info_action = nullptr;

    GError *error = nullptr;
    char *transport_state = nullptr;
    if (!gupnp_service_proxy_end_action(proxy, action, &error,
                                        "CurrentTransportState", G_TYPE_STRING, &transport_state,
                                        nullptr)) {
        qWarning() << "GetTransportInfo for" << mr->udn() << "failed:" << error->message;
        g_error_free(error);
        return;
    }

    if (mr->transport_state != transport_state) {
        mr->transport_state = transport_state;
        Q_EMIT mr->transportStateChanged();
    }
}

void MediaRenderer::get_media_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);
    mr->get_media_info_action = nullptr;

    GError *error = nullptr;
    char *duration = nullptr;
    if (!gupnp_service_proxy_end_action(proxy, action, &error,
                                        "MediaDuration", G_TYPE_STRING, &duration,
                                        nullptr)) {
        qWarning() << "GetMediaInfo for" << mr->udn() << "failed:" << error->message;
        g_error_free(error);
        return;
    }

    if (mr->media_duration != duration) {
        mr->media_duration = duration;
        Q_EMIT mr->mediaDurationChanged();
    }
    g_free(duration);
}

void MediaRenderer::get_volume_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data) {
    auto mr = reinterpret_cast<MediaRenderer*>(user_data);
    mr->get_volume_action = nullptr;

    GError *error = nullptr;
    uint volume = 0;
    if (!gupnp_service_proxy_end_action(proxy, action, &error,
                                        "CurrentVolume", G_TYPE_UINT, &volume,
                                        nullptr)) {
        qWarning() << "GetVolume for" << mr->udn() << "failed:" << error->message;
        g_error_free(error);
        return;
    }

    if (mr->volume_ != volume) {
        mr->volume_ = volume;
        Q_EMIT mr->volumeChanged();
    }
}

bool MediaRenderer::setAVTransportURI(QString uri, QString metadata) {
    // TODO: consider making this async
    GError *error = nullptr;
    if (!gupnp_service_proxy_send_action(
            av_transport.get(), "SetAVTransportURI", &error,
            "InstanceID", G_TYPE_UINT, 0,
            "CurrentURI", G_TYPE_STRING, uri.toUtf8().constData(),
            "CurrentURIMetaData", G_TYPE_STRING, metadata.toUtf8().constData(),
            nullptr,
            nullptr)) {
        qWarning() << "SetAVTransportURI failed:" << error->message;
        g_error_free(error);
        return false;
    }
    return true;
}

bool MediaRenderer::play(int speed) {
    // TODO: consider making this async
    GError *error = nullptr;
    if (!gupnp_service_proxy_send_action(
            av_transport.get(), "Play", &error,
            "InstanceID", G_TYPE_UINT, 0,
            "Speed", G_TYPE_INT, speed,
            nullptr,
            nullptr)) {
        qWarning() << "Play failed:" << error->message;
        g_error_free(error);
        return false;
    }
    return true;
}

bool MediaRenderer::stop() {
    // TODO: consider making this async
    GError *error = nullptr;
    if (!gupnp_service_proxy_send_action(
            av_transport.get(), "Stop", &error,
            "InstanceID", G_TYPE_UINT, 0,
            nullptr,
            nullptr)) {
        qWarning() << "Stop failed:" << error->message;
        g_error_free(error);
        return false;
    }
    return true;
}

// Device properties
QString MediaRenderer::udn() {
    return QString(gupnp_device_info_get_udn(GUPNP_DEVICE_INFO(device.get())));
}

QString MediaRenderer::deviceType() {
    return QString(gupnp_device_info_get_device_type(GUPNP_DEVICE_INFO(device.get())));
}

QString MediaRenderer::friendlyName() {
    char *name = gupnp_device_info_get_friendly_name(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::manufacturer() {
    char *name = gupnp_device_info_get_manufacturer(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::manufacturerUrl() {
    char *name = gupnp_device_info_get_manufacturer_url(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::modelDescription() {
    char *name = gupnp_device_info_get_model_description(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::modelName() {
    char *name = gupnp_device_info_get_model_name(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::modelNumber() {
    char *name = gupnp_device_info_get_model_number(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::modelUrl() {
    char *name = gupnp_device_info_get_model_url(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::serialNumber() {
    char *name = gupnp_device_info_get_serial_number(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::presentationUrl() {
    char *name = gupnp_device_info_get_presentation_url(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::upc() {
    char *name = gupnp_device_info_get_upc(GUPNP_DEVICE_INFO(device.get()));
    QString result(name);
    g_free(name);
    return result;
}

QString MediaRenderer::iconUrl() {
    char *icon_url = gupnp_device_info_get_icon_url(
        GUPNP_DEVICE_INFO(device.get()), nullptr,
        32, 64, 64, true, nullptr, nullptr, nullptr, nullptr);
    QString result(icon_url);
    g_free(icon_url);
    return result;
}

QStringList MediaRenderer::protocolInfo() {
    return protocol_info;
}

QString MediaRenderer::transportState() {
    return transport_state;
}

QString MediaRenderer::mediaDuration() {
    return media_duration;
}

uint MediaRenderer::volume() {
    return volume_;
}

}
