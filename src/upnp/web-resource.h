/* -*- mode: c++ -*- */

#pragma once

#include "context.h"
#include "gobj_memory.h"

#include <glib.h>
#include <libsoup/soup.h>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>

#include <memory>
#include <vector>

namespace upnp {

class WebResource : public QObject {
    Q_OBJECT
    Q_PROPERTY(upnp::Context* context READ getContext WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QUrl contentUri READ getContentUri WRITE setContentUri NOTIFY contentUriChanged)
    Q_PROPERTY(QUrl uri READ getUri)

public:
    WebResource(QObject *parent=nullptr);
    virtual ~WebResource();

    WebResource(const WebResource&) = delete;
    WebResource &operator=(const WebResource&) = delete;

    Context *getContext() const;
    void setContext(Context *new_context);
    QString getPath() const;
    void setPath(const QString &new_path);
    QUrl getContentUri() const;
    void setContentUri(const QUrl &new_content_uri);
    QList<QList<QString>> getResponseHeaders() const;
    void setResponseHeaders(const QList<QList<QString>> &new_headers);

    QUrl getUri() const;

    Q_INVOKABLE void addHeader(const QString &key, const QString &value);
    Q_INVOKABLE void clearHeaders();

Q_SIGNALS:
    void contextChanged();
    void pathChanged();
    void contentUriChanged();

private:
    void resetHandler();
    void setupHandler();

    static void requestHandler(SoupServer *server, SoupMessage *msg,
                               const char*path, GHashTable *query,
                               SoupClientContext *context, void *user_data);

    QPointer<Context> context;
    gobj_ptr<SoupServer> server;
    QString path;
    QUrl content_uri;
    std::vector<std::pair<QString, QString>> headers;

    std::unique_ptr<GMappedFile, decltype(&g_mapped_file_unref)> mapping;
};

}
