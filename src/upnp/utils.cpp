#include "utils.h"

#include <QXmlStreamWriter>

namespace upnp {

Utils::Utils(QObject *parent)
    : QObject(parent) {
}

Utils::~Utils() = default;

QString Utils::makeDIDL(const QString &url, const QString &upnpClass,
                        const QString &protocolInfo) {
    QString didl;
    QXmlStreamWriter w(&didl);

    w.writeStartDocument();
    w.writeDefaultNamespace("urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/");
    w.writeNamespace("urn:schemas-upnp-org:metadata-1-0/upnp/", "upnp");
    w.writeNamespace("http://purl.org/dc/elements/1.1/", "dc");
    w.writeStartElement("DIDL-Lite");
    w.writeStartElement("item");
    w.writeAttribute("id", "f-0");
    w.writeAttribute("parentID", "0");
    w.writeAttribute("restricted", "0");
    w.writeTextElement("dc:title", "Picture");
    w.writeTextElement("dc:creator", "Anonymous");
    w.writeTextElement("upnp:class", upnpClass);
    w.writeStartElement("res");
    w.writeAttribute("protocolInfo", protocolInfo);
    w.writeCharacters(url);
    w.writeEndElement();
    w.writeEndElement();
    w.writeEndElement();
    w.writeEndDocument();
    return didl;
}

QString Utils::getMimeType(const QString &path) {
    return "";
}

}
