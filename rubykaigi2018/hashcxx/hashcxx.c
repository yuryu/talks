/*
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ruby.h>
#include "extconf.h"
#include "impl.h"

void Init_hashcxx();

void Init_hashcxx() {
  VALUE cHash = rb_define_class("Hashcxx", rb_cObject);
  rb_include_module(cHash, rb_mEnumerable);
  rb_define_alloc_func(cHash, allocate);
  rb_define_method(cHash, "initialize", initialize, 0);
  rb_define_method(cHash, "[]=", setter, 2);
  rb_define_method(cHash, "[]", getter, 1);
  rb_define_method(cHash, "each", each, 0);
  rb_define_method(cHash, "each_pair", each, 0);
  rb_define_method(cHash, "key?", has_key, 1);
  rb_define_method(cHash, "has_key?", has_key, 1);
  rb_define_method(cHash, "empty?", empty, 0);
}

