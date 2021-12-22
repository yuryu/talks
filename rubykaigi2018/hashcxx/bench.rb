# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require 'stackprof'
require 'benchmark'
require './hashcxx'

ITER = 1000000
SEED = 12345678
STRLEN = 20
MAX_INDEX = 2 ** 60 - 1

def do_hash_bench(h)
  random = Random.new(SEED)
  ITER.times do
    key = random.rand(MAX_INDEX)
    value = random.rand(MAX_INDEX)
    if h.key? key
      h[key] << value
    else
      h[key] = [value]
    end
  end
  # verify
  random = Random.new(SEED)
  ITER.times do
    key = random.rand(MAX_INDEX)
    value = random.rand(MAX_INDEX)
    if h[key].shift != value
      raise "integer value mismatch: key = [#{key}], value = [#{h[key]}](expected: [#{value}]"
    end
  end
end

Benchmark.bm do |x|
  x.report { do_hash_bench(Hash.new) }
  x.report { do_hash_bench(Hashcxx.new) }
end

