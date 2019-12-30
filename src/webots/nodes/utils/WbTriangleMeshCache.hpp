// Copyright 1996-2019 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WB_TRIANGLE_MESH_CACHE_HPP
#define WB_TRIANGLE_MESH_CACHE_HPP

#include "sip_hash.hpp"

#include <core/WbConfig.h>

class WbIndexedFaceSet;
class WbTriangleMesh;

namespace WbTriangleMeshCache {
  WB_LIB_EXPORT extern const highwayhash::HH_U64 SIPHASH_KEY[2];

  template<class T> uint64_t sipHash13x(const T *bytes, const int size) {
    return highwayhash::SipHash13(SIPHASH_KEY, reinterpret_cast<const char *>(bytes), size * sizeof(T));
  }

  // TriangleMeshInfo is shared by all WbIndexedFaceSet instances requiring the same WbTriangleMesh.
  struct WB_LIB_EXPORT TriangleMeshInfo {
    TriangleMeshInfo();
    explicit TriangleMeshInfo(WbTriangleMesh *triangleMesh);

    WbTriangleMesh *mTriangleMesh;
    int mNumUsers;
  };

  // Key type for an instance of WbIndexedFaceSet. Instances can share a WbTriangleMesh if their keys compare equal.
  struct WB_LIB_EXPORT IndexedFaceSetKey {
    IndexedFaceSetKey();
    explicit IndexedFaceSetKey(WbIndexedFaceSet *indexedFaceSet);

    void set(WbIndexedFaceSet *indexedFaceSet);
    bool operator==(const IndexedFaceSetKey &rhs) const;

    uint64_t mHash;
  };

  // Key hashing function required by std::unordered_map
  struct WB_LIB_EXPORT IndexedFaceSetKeyHasher {
    std::size_t operator()(const IndexedFaceSetKey &k) const;
  };

  WB_LIB_EXPORT void useTriangleMesh(WbIndexedFaceSet *user);
  WB_LIB_EXPORT void releaseTriangleMesh(WbIndexedFaceSet *user);
}  // namespace WbTriangleMeshCache

#endif
