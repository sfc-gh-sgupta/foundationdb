/*
 * OnDemandStore.h
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

#pragma once

#include "flow/Arena.h"
#include "flow/IRandom.h"
#include "flow/Platform.h"
#include "fdbserver/IKeyValueStore.h"

// Create a key value store if and only if it is actually used
class OnDemandStore : public IClosable, NonCopyable {
	std::string folder;
	UID myID;
	IKeyValueStore* store;
	Promise<Future<Void>> err;
	std::string prefix;
	void open();

public:
	OnDemandStore(std::string const& folder, UID myID, std::string const& prefix);
	~OnDemandStore();
	IKeyValueStore* get();
	bool exists() const;
	IKeyValueStore* operator->();

	Future<Void> getError() override;
	Future<Void> onClosed() override;
	void dispose() override;
	void close() override;
};
