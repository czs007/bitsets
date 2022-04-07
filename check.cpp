// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License

#include <cstdint>
#include <string>
#include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include <iomanip>
#include <random>
#include <cmath>
#include "boost_ext/dynamic_bitset_ext.hpp"
#include "bitset/Types.h"
#include "Timer.h"

using MapType = std::map<std::string, std::function<bool()>>;

using ConcurrentBitset2 = bitsets::ConcurrentBitset2;
using ConcurrentBitset = bitsets::ConcurrentBitset;
using BitsetType = bitsets::BitsetType;
using BitsetView = bitsets::BitsetView;

constexpr int MODE_CURRENT_BITSET = 1;
constexpr int MODE_BOOST_BITSET = 2;

//constexpr int N_BITS = 1024 * 1024;
constexpr int N_BITS = 16;
constexpr int N = N_BITS / 8;

const uint8_t BITS_SRC[8] = {
	0b11011001, 
	0b10010001, 
	0b01011011, 
	0b10011101, 
	0b10011000, 
	0b11011000, 
	0b10010011, 
	0b10011001, 
};

std::vector<uint8_t> DatasetL;
std::vector<uint8_t> DatasetR;
std::vector<int64_t> RandomPos;

std::string view_to_string(BitsetView & view);
void check_test(std::string func_name);

bool check_boost_concurrent(const BitsetType &l, const ConcurrentBitset &h);
bool check_boost_concurrent2(const BitsetType &l, const ConcurrentBitset2 &h);
bool check_bitset_clear();
bool check_bitset_set();
bool check_bitset_or();
bool check_bitset_or_assign();
bool check_bitset_and();
bool check_bitset_and_assign();
bool check_bitset_flip();

void prepare_dataset(){
	DatasetL.resize(N);
	DatasetR.resize(N);
	RandomPos.resize(N);

	// Seed with a real random value, if available
	std::random_device rd;
    	std::mt19937 gen(rd());
    	std::uniform_int_distribution<int> uniform_dist(0, 7);
    	std::uniform_int_distribution<int> pos_uniform_dist(0, N);

	for (int i =0;i < N; i++) {
		DatasetL[i] = BITS_SRC[uniform_dist(gen)];
		DatasetR[i] = BITS_SRC[uniform_dist(gen)];
		RandomPos[i] = pos_uniform_dist(gen);
	}
}

bool check_bitset_and(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	auto cr1 = ConcurrentBitset(N_BITS, DatasetR.data());
	auto c_1 = cl1&cr1;
	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	auto cr2 = ConcurrentBitset2(N_BITS, DatasetR.data());
	auto c_2 = cl2&cr2;

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
  	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	auto viewR = BitsetView(DatasetR.data(), N_BITS);
 	auto y = view_to_string(viewR);
  	auto br = BitsetType(y);

	auto b_1 = bl&br;
	auto flag1 = check_boost_concurrent(b_1, *c_1);
	auto flag2 = check_boost_concurrent2(b_1, *c_2);

	return flag1 && flag2;
}

bool check_bitset_and_assign(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	auto cr1 = ConcurrentBitset(N_BITS, DatasetR.data());

	cl1 &= cr1;

	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	auto cr2 = ConcurrentBitset2(N_BITS, DatasetR.data());

	cl2 &= cr2;

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
 	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	auto viewR = BitsetView(DatasetR.data(), N_BITS);
 	auto y = view_to_string(viewR);
  	auto br = BitsetType(y);

	bl &= br;

	auto flag1 = check_boost_concurrent(bl, cl1);
	auto flag2 = check_boost_concurrent2(bl, cl2);
	return flag1 && flag2;
}

bool check_bitset_clear(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	for (int j =0; j<N_BITS; j++){
	  cl1.clear(ConcurrentBitset::id_type_t(j));
	}

	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	for (int j =0; j<N_BITS; j++){
	  cl2.clear(ConcurrentBitset2::id_type_t(j));
	}

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
  	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	for (int j =0; j<N_BITS; j++){
	  bl.reset(j);
	}
	auto flag1 = check_boost_concurrent(bl, cl1);
	auto flag2 = check_boost_concurrent2(bl, cl2);
	return flag1 && flag2;
}

bool check_bitset_set(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
		for (int j =0; j<N_BITS; j++){
		  cl1.set(ConcurrentBitset::id_type_t(j));
		}

	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
		for (int j =0; j<N_BITS; j++){
		  cl2.set(ConcurrentBitset2::id_type_t(j));
		}

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
  	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	for (int j =0; j<N_BITS; j++){
	  bl.set(j);
	}

	auto flag1 = check_boost_concurrent(bl, cl1);
	auto flag2 = check_boost_concurrent2(bl, cl2);
	return flag1 && flag2;
}

bool check_bitset_or(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	auto cr1 = ConcurrentBitset(N_BITS, DatasetR.data());
	auto c_1 = cl1|cr1;
	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	auto cr2 = ConcurrentBitset2(N_BITS, DatasetR.data());
	auto c_2 = cl2|cr2;

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
  	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	auto viewR = BitsetView(DatasetR.data(), N_BITS);
 	auto y = view_to_string(viewR);
  	auto br = BitsetType(y);

	auto b_1 = bl|br;
	auto flag1 = check_boost_concurrent(b_1, *c_1);
	auto flag2 = check_boost_concurrent2(b_1, *c_2);

	return flag1 && flag2;
}



bool check_bitset_flip(){
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	auto v1 =  BitsetView(cl1);
	cl1.negate();


	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	auto v2 = BitsetView(cl2);
	cl2.negate();

	//std::cout<<"A:"<<std::string(v1)<<std::endl;
	//std::cout<<"B:"<<std::string(v2)<<std::endl;

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
  	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);
	bl.flip();

	auto flag1 = check_boost_concurrent(bl, cl1);
	auto flag2 = check_boost_concurrent2(bl, cl2);
	return flag1 && flag2;
}

bool check_bitset_or_assign() {
	auto cl1 = ConcurrentBitset(N_BITS, DatasetL.data());
	auto cr1 = ConcurrentBitset(N_BITS, DatasetR.data());

	cl1 |= cr1;

	auto cl2 = ConcurrentBitset2(N_BITS, DatasetL.data());
	auto cr2 = ConcurrentBitset2(N_BITS, DatasetR.data());

	cl2 |= cr2;

	auto viewL = BitsetView(DatasetL.data(), N_BITS);
 	auto x = view_to_string(viewL);
  	auto bl = BitsetType(x);

	auto viewR = BitsetView(DatasetR.data(), N_BITS);
 	auto y = view_to_string(viewR);
  	auto br = BitsetType(y);

	bl|= br;

	auto flag1 = check_boost_concurrent(bl, cl1);
	auto flag2 = check_boost_concurrent2(bl, cl2);
	return flag1 && flag2;
}

std::string view_to_string(BitsetView & view){

    const char one = '1';
    const char zero = '0';
    const size_t len = view.size();
    std::string s;
    s.assign(len, zero);

    for (size_t i = 0; i < len; ++i) {
        if (view.test(i)){
		s[len-i-1] = one;
	}
    }
    return s;
}

MapType CheckFuncMap = {
	{ "clear", check_bitset_clear},
	{ "set", check_bitset_set},
	{ "|", check_bitset_or},
	{ "|=", check_bitset_or_assign},
	{ "&", check_bitset_and},
	{ "&=", check_bitset_and_assign},
	{ "flip",check_bitset_flip},
};

void check_test(std::string func_name){
	auto it = CheckFuncMap.find(func_name);
	 if (it != CheckFuncMap.end()) {
		 auto func = it->second;
		 auto ret =  func();
		 auto retStr = ret ? "Equal" : "NotEqual";
		 std::cout << func_name << ":\t"<< retStr<<std::endl;
    	}else{
		std::cout<<"Not match any function!" << std::endl;
	}
}

bool check_boost_concurrent(const BitsetType &l, const ConcurrentBitset &h){
  	auto v1 =  BitsetView((uint8_t*)boost_ext::get_data(l), l.size());
	auto v2 = BitsetView(h);
	bool ret = v1 == v2;
//	std::cout << "check1:" << ret <<std::endl;
//	std::cout << "v1:"<< std::string(v1) << std::endl;
//	std::cout << "v2:"<< std::string(v2) << std::endl;
	return ret;
}

bool check_boost_concurrent2(const BitsetType &l, const ConcurrentBitset2 &h){
//	return true;
	
  	auto v1 =  BitsetView((uint8_t*)boost_ext::get_data(l), l.size());
	auto v2 = BitsetView(h);
	bool ret = v1 == v2;
//	std::cout << "check2:" << ret <<std::endl;
//	std::cout << "check2:v1:" << std::string(v1) <<std::endl;
//	std::cout << "check2:v2:" << std::string(v2) <<std::endl;
	return ret;
	
}

int main() {
  std::string func_name = "";

  prepare_dataset();
  std::cout<<"GenerateDataset done"<<std::endl;

  std::vector<std::string> keys {
	"clear",
	"flip",
	"set",
	"|=",
	"|",
	"&=",
	"&",
  };

  for (const auto & func_name : keys){
  	check_test(func_name);
  }
  return 0;
}
