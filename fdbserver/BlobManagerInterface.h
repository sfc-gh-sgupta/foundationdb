/*
 * BlobWorkerInterface.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2018 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FDBSERVER_BLOBMANAGERINTERFACE_H
#define FDBSERVER_BLOBMANAGERINTERFACE_H
#pragma once

#include "fdbclient/FDBTypes.h"
#include "fdbclient/KeyRangeMap.h"
#include "fdbrpc/Locality.h"
#include "fdbrpc/fdbrpc.h"

struct BlobManagerInterface {
	constexpr static FileIdentifier file_identifier = 369169;
	RequestStream<ReplyPromise<Void>> waitFailure;
	RequestStream<struct HaltBlobManagerRequest> haltBlobManager;
	struct LocalityData locality;

	BlobManagerInterface() {}
	explicit BlobManagerInterface(const struct LocalityData& l) : locality(l) {}

	void initEndpoints() {}
	UID id() const { return waitFailure.getEndpoint().token; }
	NetworkAddress address() const { return waitFailure.getEndpoint().getPrimaryAddress(); }
	bool operator==(const BlobManagerInterface& r) const { return id() == r.id(); }
	bool operator!=(const BlobManagerInterface& r) const { return !(*this == r); }

	template <class Archive>
	void serialize(Archive& ar) {
		serializer(ar, waitFailure, haltBlobManager, locality);
	}
};

struct HaltBlobManagerRequest {
	constexpr static FileIdentifier file_identifier = 4149140;
	UID requesterID;
	ReplyPromise<Void> reply;

	HaltBlobManagerRequest() {}
	explicit HaltBlobManagerRequest(UID uid) : requesterID(uid) {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, requesterID, reply);
	}
};

// CAN REMOVE
void updateClientBlobRanges(KeyRangeMap<bool>* knownBlobRanges,
                            RangeResult dbBlobRanges,
                            Arena a,
                            VectorRef<KeyRangeRef>* rangesToAdd,
                            VectorRef<KeyRangeRef>* rangesToRemove);

#endif
