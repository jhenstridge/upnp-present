/*
 * upnp-present: Send photos to a UPnP media renderer
 * Copyright (C) 2015  James Henstridge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "utils.h"

#include <QMimeDatabase>
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

QString Utils::getMimeType(const QUrl &url) {
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForUrl(url);
    return mime.name();
}

}
