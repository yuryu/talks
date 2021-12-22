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

require './hashcxx'

a = Hashcxx.new
p a.empty?
a[0] = 0
a[1] = 1
a[1] = 2
a['a'] = 'a'
a['b'] = 'b'
a[:x] = :x
a[:y] = :y
a[:array] = []
a[:array] << 1
a[:array] << 2
a[:array] << 3

a.each { |x, y| 
  print "a[#{x}] = #{y}\n"
}

p "has_key?", a.has_key?(0)
p a[:array].shift
p a[:array]
p a.empty?
