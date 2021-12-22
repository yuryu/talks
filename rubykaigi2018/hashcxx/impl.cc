// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "impl.h"
#include <algorithm>
#include <experimental/string_view>
#include <iostream>
#include <unordered_map>

struct ValueHash {
  size_t operator()(const VALUE& x) const {
    switch (TYPE(x)) {
      case T_FIXNUM:
        return std::hash<long>()(FIX2LONG(x));
      case T_SYMBOL:
        return std::hash<ID>()(SYM2ID(x));
      case T_STRING:
        return std::hash<std::experimental::string_view>()
            (std::experimental::string_view(RSTRING_PTR(x), RSTRING_LEN(x)));
    }

    return std::hash<long>()(FIX2LONG(rb_hash(x)));
  }
};

struct ValueComp {
  bool operator() (const VALUE& lhs, const VALUE& rhs) const {
    switch (TYPE(lhs)) {
      case T_FIXNUM:
        if (! FIXNUM_P(rhs)) break;
        return FIX2LONG(lhs) == FIX2LONG(rhs);
      case T_STRING: {
        if (TYPE(rhs) != T_STRING) break;
        if (lhs == rhs) return true;
        const auto lhss = std::experimental::string_view(RSTRING_PTR(lhs), RSTRING_LEN(rhs));
        const auto rhss = std::experimental::string_view(RSTRING_PTR(rhs), RSTRING_LEN(rhs));
        return lhss.compare(rhss) == 0;
      }
      case T_SYMBOL:
        if (! SYMBOL_P(rhs)) break;
        return SYM2ID(lhs) == SYM2ID(rhs);
    }
    return rb_funcall(lhs, rb_intern("eql?"), 1, rhs) == Qtrue;
  }
};

using ruby_hash_t = std::unordered_map<VALUE, VALUE, ValueHash, ValueComp>;

namespace {

ruby_hash_t* get_hash(VALUE self) {
  ruby_hash_t* hash;
  Data_Get_Struct(self, ruby_hash_t, hash);
  return hash;
};

void mark(void* p) {
  auto* hash = reinterpret_cast<ruby_hash_t*>(p);
  for (auto& iter: *hash) {
    rb_gc_mark(iter.second);
  }
}

void deallocate(void* hash) {
  delete reinterpret_cast<ruby_hash_t*>(hash);
}

}

VALUE allocate(VALUE klass) {
  ruby_hash_t* value = new ruby_hash_t();
  return Data_Wrap_Struct(klass, mark, deallocate, value);
}


VALUE initialize(VALUE self) {
  return self;
}

VALUE setter(VALUE self, VALUE key, VALUE value) {
  auto* hash = get_hash(self);
  (*hash)[key] = value;
  return value;
}

VALUE getter(VALUE self, VALUE key) {
  const auto* hash = get_hash(self);
  const auto i = hash->find(key);
  if (i == hash->end()) {
    return Qnil;
  }
  return i->second;
}

VALUE each(VALUE self) {
  if (rb_block_given_p()) {
    const auto* hash = get_hash(self);
    for(auto& i: *hash) {
      rb_yield_values(2, i.first, i.second);
    }
    return self;
  } else {
    return rb_funcall(self, rb_intern("enum_for"), 0);
  }
}

VALUE has_key(VALUE self, VALUE key) {
  const auto* hash = get_hash(self);
  return hash->find(key) == hash->end() ? Qfalse : Qtrue;
}

VALUE empty(VALUE self) {
  return get_hash(self)->empty() ? Qtrue : Qfalse;
}

