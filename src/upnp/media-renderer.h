/* -*- mode: c++ -*- */

#pragma once

#include "gobj_memory.h"

#include <libgupnp/gupnp-device-proxy.h>
#include <libgupnp/gupnp-service-proxy.h>

#include <QObject>
#include <QString>
#include <QStringList>

class ControlPointModel;

class MediaRenderer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString udn READ udn CONSTANT)
    Q_PROPERTY(QString deviceType READ deviceType CONSTANT)
    Q_PROPERTY(QString friendlyName READ friendlyName CONSTANT)
    Q_PROPERTY(QString manufacturer READ manufacturer CONSTANT)
    Q_PROPERTY(QString manufacturerUrl READ manufacturerUrl CONSTANT)
    Q_PROPERTY(QString modelDescription READ modelDescription CONSTANT)
    Q_PROPERTY(QString modelName READ modelName CONSTANT)
    Q_PROPERTY(QString modelNumber READ modelNumber CONSTANT)
    Q_PROPERTY(QString modelUrl READ modelUrl CONSTANT)
    Q_PROPERTY(QString serialNumber READ serialNumber CONSTANT)
    Q_PROPERTY(QString presentationUrl READ presentationUrl CONSTANT)
    Q_PROPERTY(QString upc READ upc CONSTANT)
    Q_PROPERTY(QString iconUrl READ iconUrl CONSTANT)

    Q_PROPERTY(QStringList protocolInfo READ protocolInfo NOTIFY protocolInfoChanged)
    Q_PROPERTY(QString transportState READ transportState NOTIFY transportStateChanged)
    Q_PROPERTY(QString mediaDuration READ mediaDuration NOTIFY mediaDurationChanged)
    Q_PROPERTY(uint volume READ volume NOTIFY volumeChanged)

    friend ControlPointModel;

public:
    MediaRenderer(const gobj_ptr<GUPnPDeviceProxy> &device, QObject *parent=nullptr);
    virtual ~MediaRenderer();

    MediaRenderer(const MediaRenderer&) = delete;
    MediaRenderer &operator=(const MediaRenderer&) = delete;

    Q_INVOKABLE bool setAVTransportURI(QString uri, QString metadata);
    Q_INVOKABLE bool play(int speed=1);

    // Device properties
    QString udn();
    QString deviceType();
    QString friendlyName();
    QString manufacturer();
    QString manufacturerUrl();
    QString modelDescription();
    QString modelName();
    QString modelNumber();
    QString modelUrl();
    QString serialNumber();
    QString presentationUrl();
    QString upc();
    QString iconUrl();

    QStringList protocolInfo();
    QString transportState();
    QString mediaDuration();
    uint volume();

Q_SIGNALS:
    void protocolInfoChanged();
    void transportStateChanged();
    void mediaDurationChanged();
    void volumeChanged();

private:
    gobj_ptr<GUPnPDeviceProxy> device;
    gobj_ptr<GUPnPServiceProxy> connection_manager;
    gobj_ptr<GUPnPServiceProxy> av_transport;
    gobj_ptr<GUPnPServiceProxy> rendering_control;

    GUPnPServiceProxyAction *get_protocol_info_action = nullptr;
    GUPnPServiceProxyAction *get_transport_info_action = nullptr;
    GUPnPServiceProxyAction *get_media_info_action = nullptr;
    GUPnPServiceProxyAction *get_volume_action = nullptr;

    QStringList protocol_info;
    QString transport_state;
    QString media_duration;
    uint volume_ = 0;

    static void av_transport_last_change_cb(GUPnPServiceProxy *proxy, const char *variable, GValue *value, void *user_data);
    static void rendering_control_last_change_cb(GUPnPServiceProxy *proxy, const char *variable, GValue *value, void *user_data);

    static void get_protocol_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data);
    static void get_transport_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data);
    static void get_media_info_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data);
    static void get_volume_cb(GUPnPServiceProxy *proxy, GUPnPServiceProxyAction *action, void *user_data);
};
