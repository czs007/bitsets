// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License.

#include <cstring>
#include <string>
#include <memory>
#include "Bitset.h"
#include "BitsetView.h"

namespace faiss {

ConcurrentBitset&
ConcurrentBitset::operator&=(const ConcurrentBitset& bitset) {
	auto u8_1 = mutable_data();
	auto u8_2 = bitset.data();
	auto u64_1 = reinterpret_cast<uint64_t*>(u8_1);
	auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

	size_t n8 = bitset_.size();
	size_t n64 = n8 / 8;

	for (size_t i = 0; i < n64; i++) {
	    u64_1[i] &= u64_2[i];
	}

	size_t remain = n8 % 8;
	u8_1 += n64 * 8;
	u8_2 += n64 * 8;
	for (size_t i = 0; i < remain; i++) {
	    u8_1[i] &= u8_2[i];
	}

	return *this;
}

ConcurrentBitset&
ConcurrentBitset::operator&=(const BitsetView& view) {
	auto u8_1 = mutable_data();
	auto u8_2 = view.data();
	auto u64_1 = reinterpret_cast<uint64_t*>(u8_1);
	auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

	size_t n8 = bitset_.size();
	size_t n64 = n8 / 8;

	for (size_t i = 0; i < n64; i++) {
	    u64_1[i] &= u64_2[i];
	}

	size_t remain = n8 % 8;
	u8_1 += n64 * 8;
	u8_2 += n64 * 8;
	for (size_t i = 0; i < remain; i++) {
	    u8_1[i] &= u8_2[i];
	}

	return *this;
}

std::shared_ptr<ConcurrentBitset>
ConcurrentBitset::operator&(const ConcurrentBitset& bitset) const {
auto result_bitset = std::make_shared<ConcurrentBitset>(bitset.size());

auto result_8 = result_bitset->mutable_data();
auto result_64 = reinterpret_cast<uint64_t*>(result_8);

auto u8_1 = data();
auto u8_2 = bitset.data();
auto u64_1 = reinterpret_cast<const uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    result_64[i] = u64_1[i] & u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
result_8 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    result_8[i] = u8_1[i] & u8_2[i];
}

return result_bitset;
}

std::shared_ptr<ConcurrentBitset>
ConcurrentBitset::operator&(const BitsetView& view) const {
auto result_bitset = std::make_shared<ConcurrentBitset>(view.size());

auto result_8 = result_bitset->mutable_data();
auto result_64 = reinterpret_cast<uint64_t*>(result_8);

auto u8_1 = data();
auto u8_2 = view.data();
auto u64_1 = reinterpret_cast<const uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    result_64[i] = u64_1[i] & u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
result_8 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    result_8[i] = u8_1[i] & u8_2[i];
}

return result_bitset;
}


ConcurrentBitset&
ConcurrentBitset::operator|=(const ConcurrentBitset& bitset) {
auto u8_1 = mutable_data();
auto u8_2 = bitset.data();
auto u64_1 = reinterpret_cast<uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    u64_1[i] |= u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    u8_1[i] |= u8_2[i];
}

return *this;
}

ConcurrentBitset&
ConcurrentBitset::operator|=(const BitsetView& view) {
auto u8_1 = mutable_data();
auto u8_2 = view.data();
auto u64_1 = reinterpret_cast<uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    u64_1[i] |= u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    u8_1[i] |= u8_2[i];
}

return *this;
}


std::shared_ptr<ConcurrentBitset>
ConcurrentBitset::operator|(const ConcurrentBitset& bitset) const {
auto result_bitset = std::make_shared<ConcurrentBitset>(bitset.size());

auto result_8 = result_bitset->mutable_data();
auto result_64 = reinterpret_cast<uint64_t*>(result_8);

auto u8_1 = data();
auto u8_2 = bitset.data();
auto u64_1 = reinterpret_cast<const uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    result_64[i] = u64_1[i] | u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
result_8 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    result_8[i] = u8_1[i] | u8_2[i];
}

return result_bitset;
}

std::shared_ptr<ConcurrentBitset>
ConcurrentBitset::operator|(const BitsetView& view) const {
auto result_bitset = std::make_shared<ConcurrentBitset>(view.size());

auto result_8 = result_bitset->mutable_data();
auto result_64 = reinterpret_cast<uint64_t*>(result_8);

auto u8_1 = data();
auto u8_2 = view.data();
auto u64_1 = reinterpret_cast<const uint64_t*>(u8_1);
auto u64_2 = reinterpret_cast<const uint64_t*>(u8_2);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    result_64[i] = u64_1[i] | u64_2[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
u8_2 += n64 * 8;
result_8 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    result_8[i] = u8_1[i] | u8_2[i];
}

return result_bitset;
}


ConcurrentBitset&
ConcurrentBitset::negate() {
auto u8_1 = mutable_data();
auto u64_1 = reinterpret_cast<uint64_t*>(u8_1);

size_t n8 = bitset_.size();
size_t n64 = n8 / 8;

for (size_t i = 0; i < n64; i++) {
    u64_1[i] = ~u64_1[i];
}

size_t remain = n8 % 8;
u8_1 += n64 * 8;
for (size_t i = 0; i < remain; i++) {
    u8_1[i] = ~u8_1[i];
}

return *this;
}

/*
bool 
ConcurrentBitset::all() const{

auto p_data = reinterpret_cast<const uint64_t *>(data());
auto len = size() >> 3;

auto popcount8 = [&](uint8_t x) -> int{
    x = (x & 0x55) + ((x >> 1) & 0x55);
    x = (x & 0x33) + ((x >> 2) & 0x33);
    x = (x & 0x0F) + ((x >> 4) & 0x0F);
    return x;
};

for (size_t i = 0; i < len; ++i) {
	if(*p_data){
		return true;
	}
}

auto p_byte = data() + (len << 3);
for (auto i = (len << 3); i < size(); ++i) {
    if(*p_byte) {
	return true;
    }
    p_byte++;
}
return false;

}


bool 
ConcurrentBitset::any() const{

auto p_data = reinterpret_cast<const uint64_t *>(data());
auto len = size() >> 3;

auto popcount8 = [&](uint8_t x) -> int{
    x = (x & 0x55) + ((x >> 1) & 0x55);
    x = (x & 0x33) + ((x >> 2) & 0x33);
    x = (x & 0x0F) + ((x >> 4) & 0x0F);
    return x;
};

for (size_t i = 0; i < len; ++i) {
	if(*p_data){
		return true;
	}
}

auto p_byte = data() + (len << 3);
for (auto i = (len << 3); i < size(); ++i) {
    if(*p_byte) {
	return true;
    }
    p_byte++;
}
return false;

}
*/

size_t
ConcurrentBitset::count() const {
size_t ret = 0;
auto p_data = reinterpret_cast<const uint64_t *>(data());
auto len = size() >> 3;
//auto remainder = size() % 8;
auto popcount8 = [&](uint8_t x) -> int{
    x = (x & 0x55) + ((x >> 1) & 0x55);
    x = (x & 0x33) + ((x >> 2) & 0x33);
    x = (x & 0x0F) + ((x >> 4) & 0x0F);
    return x;
};
for (size_t i = 0; i < len; ++i) {
    ret += __builtin_popcountl(*p_data);
    p_data++;
}
auto p_byte = data() + (len << 3);
for (auto i = (len << 3); i < size(); ++i) {
    ret += popcount8(*p_byte);
    p_byte++;
}
return ret;
}

ConcurrentBitset::operator std::string() const { 
    const char one = '1';
    const char zero = '0';
    const size_t len = size();
    std::string s;
    s.assign (len, zero);

    for (size_t i = 0; i < len; ++i) {
        if (test(id_type_t(i)))
            s.assign(len - 1 - i, one);
    }
    return s;
}

bool operator==(const ConcurrentBitset& lhs, const ConcurrentBitset& rhs) {
    if (std::addressof(lhs) == std::addressof(rhs)){
	return true;
    }

    if (lhs.size() != rhs.size()){
	return false;
    }

    if (lhs.byte_size() != rhs.byte_size()){
	return false;
    }


    auto ret = std::memcmp(lhs.data(), rhs.data(), lhs.byte_size());
    return ret == 0;
}

bool operator!=(const ConcurrentBitset& lhs, const ConcurrentBitset& rhs){ 
    return !(lhs == rhs); 
}

std::ostream& operator<<(std::ostream& os, const ConcurrentBitset& bitset)
{
    os << std::string(bitset);
    return os;
}


}  // namespace faiss
