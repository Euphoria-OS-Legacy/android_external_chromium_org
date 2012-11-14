// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/quic_client_session.h"

#include <vector>

#include "base/stl_util.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/test_tools/quic_test_utils.h"

using testing::_;

namespace net {
namespace test {
namespace {

class QuicClientSessionTest : public ::testing::Test {
 protected:
  QuicClientSessionTest()
      : guid_(1),
        connection_(new PacketSavingConnection(guid_, IPEndPoint())),
        session_(connection_) {
  }

 protected:
  QuicGuid guid_;
  PacketSavingConnection* connection_;
  QuicClientSession session_;
  QuicConnectionVisitorInterface* visitor_;
};

TEST_F(QuicClientSessionTest, CryptoConnect) {
  session_.CryptoConnect();
  ASSERT_EQ(1u, connection_->packets_.size());
  scoped_ptr<QuicPacket> chlo(ConstructHandshakePacket(guid_, kCHLO));
  CompareQuicDataWithHexError("CHLO", connection_->packets_[0], chlo.get());
}

TEST_F(QuicClientSessionTest, MaxNumConnections) {
  // Initialize crypto before the client session will create a stream.
  session_.CryptoConnect();
  ASSERT_EQ(1u, connection_->packets_.size());
  scoped_ptr<QuicPacket> chlo(ConstructHandshakePacket(guid_, kCHLO));
  CompareQuicDataWithHexError("CHLO", connection_->packets_[0], chlo.get());
  // Simulate the server crypto handshake.
  CryptoHandshakeMessage server_message;
  server_message.tag = kSHLO;
  session_.GetCryptoStream()->OnHandshakeMessage(server_message);

  std::vector<QuicReliableClientStream*> streams;
  for (size_t i = 0; i < kDefaultMaxStreamsPerConnection; i++) {
    QuicReliableClientStream* stream = session_.CreateOutgoingReliableStream();
    streams.push_back(stream);
    EXPECT_TRUE(stream);
  }
  EXPECT_FALSE(session_.CreateOutgoingReliableStream());

  // Close a stream and ensure I can now open a new one.
  session_.CloseStream(streams[0]->id());
  scoped_ptr<QuicReliableClientStream> stream(
      session_.CreateOutgoingReliableStream());
  EXPECT_TRUE(stream.get());
  STLDeleteElements(&streams);
}

}  // namespace
}  // namespace test
}  // namespace net