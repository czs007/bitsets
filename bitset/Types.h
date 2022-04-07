// Licensed to the LF AI & Data foundation under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include <boost/align/aligned_allocator.hpp>
#include <boost/dynamic_bitset.hpp>

#include "BitsetView.h"
#include "Bitset2.h"
#include "Bitset.h"

namespace bitsets {

using ConcurrentBitset = faiss::ConcurrentBitset;
using ConcurrentBitset2 = faiss::ConcurrentBitset2;
using ConcurrentBitsetPtr = faiss::ConcurrentBitsetPtr;
using ConcurrentBitset2Ptr = faiss::ConcurrentBitset2Ptr;

// NOTE: dependent type
// used at meta-template programming
template <class...>
constexpr std::true_type always_true{};

template <class...>
constexpr std::false_type always_false{};

template <typename T>
using aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 64>>;

using BitsetView = faiss::BitsetView;

using BitsetType = boost::dynamic_bitset<>;
using BitsetTypeOpt = std::optional<BitsetType>;

}  // namespace bitsets
