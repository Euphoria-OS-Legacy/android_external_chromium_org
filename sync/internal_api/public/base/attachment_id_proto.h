// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_ATTACHMENT_ID_PROTO_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_ATTACHMENT_ID_PROTO_H_

#include "sync/base/sync_export.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

// Helper functions that are logically part of sync_pb::AttachmentIdProto.

// Creates a unique AttachmentIdProto.
SYNC_EXPORT_PRIVATE sync_pb::AttachmentIdProto CreateAttachmentIdProto();

}  // namespace syncer

#endif  // SYNC_INTERNAL_API_PUBLIC_BASE_ATTACHMENT_ID_PROTO_H_
