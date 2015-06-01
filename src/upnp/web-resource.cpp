#include "web-resource.h"

#include <QDebug>
#include <string>

namespace upnp {

WebResource::WebResource(QObject *parent)
    : QObject(parent), mapping(nullptr, g_mapped_file_unref) {
}

WebResource::~WebResource() {
    resetHandler();
}

void WebResource::resetHandler() {
    if (server) {
        soup_server_remove_handler(server.get(), path.toUtf8().constData());
        server.reset();
    }
}

void WebResource::setupHandler() {
    if (context && !path.isEmpty()) {
        SoupServer *s = gupnp_context_get_server(context->context.get());
        g_object_ref(s);
        server.reset(s);

        soup_server_add_handler(s, path.toUtf8().constData(),
                                &WebResource::requestHandler, this, nullptr);
    }
}


Context *WebResource::getContext() const {
    return context;
}

void WebResource::setContext(Context *new_context) {
    if (context == new_context) return;

    resetHandler();
    context = new_context;
    setupHandler();
    Q_EMIT contextChanged();
}

QString WebResource::getPath() const {
    return path;
}

void WebResource::setPath(const QString &new_path) {
    if (path == new_path) return;

    resetHandler();
    path = new_path;
    setupHandler();
    Q_EMIT pathChanged();
}

QUrl WebResource::getContentUri() const {
    return content_uri;
}

void WebResource::setContentUri(const QUrl &new_content_uri) {
    if (content_uri == new_content_uri) return;

    mapping.reset();
    content_uri = new_content_uri;
    Q_EMIT contentUriChanged();
    if (!content_uri.isValid()) {
        return;
    }

    if (!content_uri.isLocalFile()) {
        qWarning() << "Content URI should be a local file, but got" << content_uri;
        return;
    }

    GError *error = nullptr;
    mapping.reset(
        g_mapped_file_new(content_uri.toLocalFile().toUtf8().constData(),
                          false, &error));
    if (!mapping) {
        qWarning() << "Failed to map file:" << error->message;
        g_error_free(error);
    }
}

QUrl WebResource::getUri() const {
    QUrl uri;

    if (!context) return uri;

    uri.setScheme("http");
    uri.setHost(gupnp_context_get_host_ip(context->context.get()));
    uri.setPort(gupnp_context_get_port(context->context.get()));
    uri.setPath(path);
    return uri;
}

void WebResource::requestHandler(SoupServer *server, SoupMessage *msg,
                                 const char*path, GHashTable *query,
                                 SoupClientContext *context, void *user_data) {
    auto res = reinterpret_cast<WebResource*>(user_data);

    if (msg->method != SOUP_METHOD_GET && msg->method != SOUP_METHOD_HEAD) {
        soup_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED);
        return;
    }

    if (!res->mapping) {
        soup_message_set_status(msg, SOUP_STATUS_INTERNAL_SERVER_ERROR);
        return;
    }

    auto length = g_mapped_file_get_length(res->mapping.get());
    soup_message_headers_append(
        msg->response_headers, "Content-Length", std::to_string(length).c_str());
    // Add custom response headers

    if (msg->method == SOUP_METHOD_GET) {
        SoupBuffer *buffer = soup_buffer_new_with_owner(
            g_mapped_file_get_contents(res->mapping.get()),
            g_mapped_file_get_length(res->mapping.get()),
            g_mapped_file_ref(res->mapping.get()),
            (GDestroyNotify)g_mapped_file_unref);
        soup_message_body_append_buffer(msg->response_body, buffer);
        soup_buffer_free(buffer);
    }
    soup_message_set_status(msg, SOUP_STATUS_OK);
}

}
