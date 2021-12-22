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

#ifndef HASH_CXX_IMPL_H__
#define HASH_CXX_IMPL_H__

#include <ruby.h>

#ifdef __cplusplus

extern "C" {

#endif

VALUE allocate(VALUE klass);
VALUE initialize(VALUE self);
VALUE setter(VALUE self, VALUE key, VALUE value);
VALUE getter(VALUE self, VALUE key);
VALUE each(VALUE self);
VALUE has_key(VALUE self, VALUE key);
VALUE empty(VALUE self);

#ifdef __cplusplus

} // extern "C"

#endif

#endif
