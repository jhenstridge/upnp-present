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

#include "protocol-info.h"

namespace upnp {

ProtocolInfo::ProtocolInfo(QObject *parent)
    : QObject(parent), info(gupnp_protocol_info_new()) {
}

ProtocolInfo::~ProtocolInfo() = default;

QString ProtocolInfo::protocol() const {
    return gupnp_protocol_info_get_protocol(info.get());
}

void ProtocolInfo::setProtocol(const QString &new_protocol) {
    gupnp_protocol_info_set_protocol(info.get(), new_protocol.toUtf8().constData());
}

QString ProtocolInfo::network() const {
    return gupnp_protocol_info_get_network(info.get());
}

void ProtocolInfo::setNetwork(const QString &new_network) {
    gupnp_protocol_info_set_network(info.get(), new_network.toUtf8().constData());
}

QString ProtocolInfo::mimeType() const {
    return gupnp_protocol_info_get_mime_type(info.get());
}

void ProtocolInfo::setMimeType(const QString &new_mime_type) {
    gupnp_protocol_info_set_mime_type(info.get(), new_mime_type.toUtf8().constData());
}

QString ProtocolInfo::dlnaProfile() const {
    return gupnp_protocol_info_get_dlna_profile(info.get());
}

void ProtocolInfo::setDlnaProfile(const QString &new_profile) {
    gupnp_protocol_info_set_dlna_profile(info.get(), new_profile.toUtf8().constData());
}

ProtocolInfo::DlnaConversion ProtocolInfo::dlnaConversion() const {
    return static_cast<DlnaConversion>(gupnp_protocol_info_get_dlna_conversion(info.get()));
}

void ProtocolInfo::setDlnaConversion(DlnaConversion new_conversion) {
    gupnp_protocol_info_set_dlna_conversion(info.get(), static_cast<GUPnPDLNAConversion>(new_conversion));
}

ProtocolInfo::DlnaOperations ProtocolInfo::dlnaOperation() const {
    return static_cast<DlnaOperation>(gupnp_protocol_info_get_dlna_operation(info.get()));
}
void ProtocolInfo::setDlnaOperation(DlnaOperations new_operation) {
    gupnp_protocol_info_set_dlna_operation(info.get(), static_cast<GUPnPDLNAOperation>(int(new_operation)));
}

ProtocolInfo::DlnaFlags ProtocolInfo::dlnaFlags() const {
    return static_cast<DlnaFlag>(gupnp_protocol_info_get_dlna_flags(info.get()));
}

void ProtocolInfo::setDlnaFlags(DlnaFlags new_flags) {
    gupnp_protocol_info_set_dlna_flags(info.get(), static_cast<GUPnPDLNAFlags>(int(new_flags)));
}

bool ProtocolInfo::isCompatible(const upnp::ProtocolInfo *other) {
    return gupnp_protocol_info_is_compatible(info.get(), other->info.get());
}

}
