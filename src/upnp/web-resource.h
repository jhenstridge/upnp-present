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

namespace upnp {

class WebResource : public QObject {
    Q_OBJECT
    Q_PROPERTY(upnp::Context* context READ getContext WRITE setContext)
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    Q_PROPERTY(QUrl contentUri READ getContentUri WRITE setContentUri)
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

    QUrl getUri() const;

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

    std::unique_ptr<GMappedFile, decltype(&g_mapped_file_unref)> mapping;
};

}
