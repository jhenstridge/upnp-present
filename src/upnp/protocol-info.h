/* -*- mode: c++ -*- */
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

#pragma once

#include "gobj_memory.h"

#include <libgupnp-av/gupnp-protocol-info.h>
#include <QFlags>
#include <QObject>
#include <QString>

#include <memory>
#include <vector>

namespace upnp {

class ProtocolInfo : public QObject {
    Q_OBJECT
    Q_ENUMS(DlnaConversion)
    Q_FLAGS(DlnaOperations DlnaFlags)
    Q_PROPERTY(QString protocol READ protocol WRITE setProtocol)
    Q_PROPERTY(QString network READ network WRITE setNetwork)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType)
    Q_PROPERTY(QString dlnaProfile READ dlnaProfile WRITE setDlnaProfile)
    Q_PROPERTY(DlnaConversion dlnaConversion READ dlnaConversion WRITE setDlnaConversion)
    Q_PROPERTY(DlnaOperations dlnaOperation READ dlnaOperation WRITE setDlnaOperation)
    Q_PROPERTY(DlnaFlags dlnaFlags READ dlnaFlags WRITE setDlnaFlags)

public:
    enum DlnaConversion {
        None = GUPNP_DLNA_CONVERSION_NONE,
        Transcoded = GUPNP_DLNA_CONVERSION_TRANSCODED,
    };

    enum DlnaOperation {
        Range = GUPNP_DLNA_OPERATION_RANGE,
        TimeSeek = GUPNP_DLNA_OPERATION_TIMESEEK,
    };
    Q_DECLARE_FLAGS(DlnaOperations, DlnaOperation)

    enum DlnaFlag {
        SenderPaced = GUPNP_DLNA_FLAGS_SENDER_PACED,
        TimeBasedSeek = GUPNP_DLNA_FLAGS_TIME_BASED_SEEK,
        ByteBasedSeek = GUPNP_DLNA_FLAGS_BYTE_BASED_SEEK,
        PlayContainer = GUPNP_DLNA_FLAGS_PLAY_CONTAINER,
        S0Increase = GUPNP_DLNA_FLAGS_S0_INCREASE,
        SnIncrease = GUPNP_DLNA_FLAGS_SN_INCREASE,
        RtspPause = GUPNP_DLNA_FLAGS_RTSP_PAUSE,
        StreamingTransferMode = GUPNP_DLNA_FLAGS_STREAMING_TRANSFER_MODE,
        InteractiveTransferMode = GUPNP_DLNA_FLAGS_INTERACTIVE_TRANSFER_MODE,
        BackgroundTransferMode = GUPNP_DLNA_FLAGS_BACKGROUND_TRANSFER_MODE,
        ConnectionStall = GUPNP_DLNA_FLAGS_CONNECTION_STALL,
        DlnaV15 = GUPNP_DLNA_FLAGS_DLNA_V15,
        LinkProtectedContent = GUPNP_DLNA_FLAGS_LINK_PROTECTED_CONTENT,
        ClearTextByteSeekFull = GUPNP_DLNA_FLAGS_CLEAR_TEXT_BYTE_SEEK_FULL,
        LopClearTextByteSeek = GUPNP_DLNA_FLAGS_LOP_CLEAR_TEXT_BYTE_SEEK,
    };
    Q_DECLARE_FLAGS(DlnaFlags, DlnaFlag)

    ProtocolInfo(QObject *parent=nullptr);
    ProtocolInfo(const QString &protocol_info, QObject *parent=nullptr);
    virtual ~ProtocolInfo();

    ProtocolInfo(const ProtocolInfo&) = delete;
    ProtocolInfo &operator=(const ProtocolInfo&) = delete;

    QString protocol() const;
    void setProtocol(const QString &new_protocol);
    QString network() const;
    void setNetwork(const QString &new_network);
    QString mimeType() const;
    void setMimeType(const QString &new_mime_type);
    QString dlnaProfile() const;
    void setDlnaProfile(const QString &new_profile);
    DlnaConversion dlnaConversion() const;
    void setDlnaConversion(DlnaConversion new_conversion);
    DlnaOperations dlnaOperation() const;
    void setDlnaOperation(DlnaOperations new_operation);
    DlnaFlags dlnaFlags() const;
    void setDlnaFlags(DlnaFlags new_flags);

    Q_INVOKABLE bool isCompatible(upnp::ProtocolInfo *other) const;
    Q_INVOKABLE QString asString() const;

private:
    gobj_ptr<GUPnPProtocolInfo> info;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(upnp::ProtocolInfo::DlnaOperations)
Q_DECLARE_OPERATORS_FOR_FLAGS(upnp::ProtocolInfo::DlnaFlags)
