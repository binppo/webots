// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "StaticMesh.hpp"

#include "Config.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "Primitive.hpp"

#include <wren/static_mesh.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <unordered_map>
#include <vector>

namespace wren {

  std::unordered_map<cache::Key, cache::MeshData> StaticMesh::cCache;

  bool StaticMesh::createOrRetrieveFromCache(StaticMesh **mesh, const cache::Key key) {
    (*mesh) = StaticMesh::createStaticMesh();
    (*mesh)->mCacheKey = key;

    auto it = StaticMesh::cCache.find((*mesh)->mCacheKey);
    if (it != StaticMesh::cCache.end()) {
      (*mesh)->mCacheData = &(it->second);
      ++(*mesh)->mCacheData->mNumUsers;
      return true;
    } else {
      StaticMesh::cCache.emplace((*mesh)->mCacheKey, cache::MeshData{});
      (*mesh)->mCacheData = &(StaticMesh::cCache.at((*mesh)->mCacheKey));
    }

    return false;
  }

  StaticMesh *StaticMesh::createUnitBox(bool outline) {
    StaticMesh *mesh;

    if (outline) {
      const cache::Key key(cache::sipHash13c("BoxOutline", strlen("BoxOutline")));

      if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
        return mesh;

      const int vertexCounter = 8;
      const int indexCounter = 24;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.5f));

      mesh->addIndex(0);
      mesh->addIndex(1);
      mesh->addIndex(1);
      mesh->addIndex(2);
      mesh->addIndex(2);
      mesh->addIndex(3);
      mesh->addIndex(3);
      mesh->addIndex(0);

      mesh->addIndex(0);
      mesh->addIndex(4);
      mesh->addIndex(1);
      mesh->addIndex(5);
      mesh->addIndex(2);
      mesh->addIndex(6);
      mesh->addIndex(3);
      mesh->addIndex(7);

      mesh->addIndex(4);
      mesh->addIndex(5);
      mesh->addIndex(5);
      mesh->addIndex(6);
      mesh->addIndex(6);
      mesh->addIndex(7);
      mesh->addIndex(7);
      mesh->addIndex(4);
    } else {
      const cache::Key key(cache::sipHash13c("Box", strlen("Box")));
      if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
        return mesh;

      const int vertexCounter = 24;
      const int indexCounter = 36;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      // left
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, -0.5f));
      // back
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.5f));
      // bottom
      mesh->addCoord(glm::vec3(0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, -0.5f));
      // right
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, -0.5f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, -0.5f));
      // front
      mesh->addCoord(glm::vec3(0.5f, 0.5f, -0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, -0.5f));
      // top
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.5f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.5f));
      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.5f));

      // left
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(0.0f, 1.0f, 0.0f));
      // back
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
      // bottom
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
      // right
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(0.0f, -1.0f, 0.0f));
      // front
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(1.0f, 0.0f, 0.0f));
      // top
      for (int i = 0; i < 4; ++i)
        mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));

      // left
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 1.0f));
      // back
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 0.5f));
      // bottom
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 0.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 0.5f));
      // right
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 1.0f));
      // front
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 1.0f));
      // top
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 0.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.5f, 0.5f));

      unsigned int i = 0;
      while (i < vertexCounter) {
        mesh->addIndex(i + 0);
        mesh->addIndex(i + 1);
        mesh->addIndex(i + 2);

        mesh->addIndex(i + 0);
        mesh->addIndex(i + 2);
        mesh->addIndex(i + 3);
        i += 4;
      }
    }

    // bounding volumes
    const primitive::Box box;
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = box.computeBoundingSphere();
      mesh->mCacheData->mAabb = primitive::Aabb(glm::vec3(-0.5f), glm::vec3(0.5f));
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createUnitCone(int subdivision, bool hasSide, bool hasBottom) {
    // hash all params with in a single call to sipHash13c
    constexpr int paramsSize = sizeof(int) + sizeof(bool) * 2;
    char params[paramsSize];
    char *dest = &params[0];

    memcpy(dest, reinterpret_cast<const void *>(&subdivision), sizeof(int));
    dest += sizeof(int);
    memcpy(dest, reinterpret_cast<const void *>(&hasSide), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&hasBottom), sizeof(bool));

    const cache::Key key(cache::sipHash13c(&params[0], paramsSize));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    const int sub1 = subdivision + 1;
    const float h = 0.5f;
    const float invSub = 1.0f / subdivision;
    const float k = invSub * glm::pi<float>() * 2.0f;

    int vertexCounter = 0;
    if (hasSide)
      vertexCounter += 2 * sub1;
    if (hasBottom)
      vertexCounter += sub1 + 1;  // circle + center

    int indexCounter = 0;
    if (hasSide)
      indexCounter += subdivision * 3;
    if (hasBottom)
      indexCounter += subdivision * 3;

    mesh->estimateVertexCount(vertexCounter);
    mesh->estimateIndexCount(indexCounter);

    if (hasSide) {
      float hh = 0.5f;
      const float rr = glm::sqrt(1.0f + hh * hh);
      hh /= rr;

      for (int i = 0; i < sub1; ++i) {
        const float alpha = k * i;
        const float beta = k * (0.5f + i);
        const float x = glm::sin(alpha);
        const float y = glm::cos(alpha);
        const float xM = glm::sin(beta);
        const float yM = glm::cos(beta);

        // bottom point
        mesh->addCoord(glm::vec3(x, y, -h));
        // apex
        mesh->addCoord(glm::vec3(0.0f, 0.0f, h));

        const float invRr = 1.0f / rr;
        mesh->addNormal(glm::vec3(x * invRr, y * invRr, hh));
        mesh->addNormal(glm::vec3(xM * invRr, yM * invRr, hh));

        const float d1 = (subdivision - i - 1) * invSub;
        const float d2 = (subdivision - i) * invSub;
        const float d = d1 + d2;
        mesh->addTexCoord(glm::vec2(d1, 1.0f));
        mesh->addTexCoord(glm::vec2(d * 0.5f, 0.0f));

        mesh->addUnwrappedTexCoord(glm::vec2(d1 * 0.5f, 1.0f));
        mesh->addUnwrappedTexCoord(glm::vec2(d * 0.25f, 0.0f));
      }

      for (int i = 0, start = mesh->indices().size(); i < subdivision; ++i, start += 2) {
        mesh->addIndex(start);
        mesh->addIndex(start + 1);
        mesh->addIndex(start + 2);
      }
    }

    if (hasBottom) {
      // define bottom center point
      const int center = mesh->coords().size();
      mesh->addCoord(glm::vec3(0.0f, 0.0f, -h));
      mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
      mesh->addTexCoord(glm::vec2(0.5f, 0.5f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.5f));

      // define bottom circle points
      for (int i = 0; i < sub1; ++i) {
        const float alpha = k * i;
        const float x = glm::sin(alpha);
        const float y = -glm::cos(alpha);
        mesh->addCoord(glm::vec3(x, -y, -h));
        mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
        mesh->addTexCoord(glm::vec2(0.5f * x + 0.5f, -0.5f * y + 0.5f));
        mesh->addUnwrappedTexCoord(glm::vec2(0.25f * x + 0.75f, -0.5f * y + 0.5f));
      }

      // create triangles to center
      for (int i = 0, start = center + 1; i < subdivision; ++i, ++start) {
        mesh->addIndex(start);
        mesh->addIndex(start + 1);
        mesh->addIndex(center);
      }
    }

    // bounding volumes
    const primitive::Cone cone(gVec3Zeros, 1.0f, 1.0f, hasSide, hasBottom);
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = cone.computeBoundingSphere();
      mesh->mCacheData->mAabb = primitive::Aabb(glm::vec3(-0.5f), glm::vec3(0.5f));
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createUnitCylinder(int subdivision, bool hasSide, bool hasTop, bool hasBottom, bool outline) {
    // hash all params with in a single call to sipHash13c
    constexpr int paramsSize = sizeof(int) + sizeof(bool) * 4;
    char params[paramsSize];
    char *dest = &params[0];

    memcpy(dest, reinterpret_cast<const void *>(&subdivision), sizeof(int));
    dest += sizeof(int);
    memcpy(dest, reinterpret_cast<const void *>(&hasSide), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&hasTop), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&hasBottom), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&outline), sizeof(bool));

    const cache::Key key(cache::sipHash13c(&params[0], paramsSize));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    if (outline) {
      const int vertexCounter = subdivision * 2;
      const int indexCounter = subdivision * 3;
      const double h = 0.5;
      const double k = (2.0 * glm::pi<float>()) / subdivision;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      for (int i = 0; i < subdivision + 1; ++i) {
        const double x = glm::sin(i * k + glm::pi<float>());
        const double y = glm::cos(i * k + glm::pi<float>());

        mesh->addCoord(glm::vec3(x, y, -h));
        mesh->addCoord(glm::vec3(x, y, h));

        const int index = i * 2;
        mesh->addIndex(index);
        mesh->addIndex(index + 1);
      }

      for (int i = 0; i < vertexCounter; ++i) {
        mesh->addIndex(i);
        mesh->addIndex(i + 2);
      }

    } else {
      const int sub1 = subdivision + 1;
      const float h = 0.5f;

      int vertexCounter = 0;
      if (hasSide)
        vertexCounter += 2 * sub1;
      if (hasTop)
        vertexCounter += sub1 + 1;  // circle + center
      if (hasBottom)
        vertexCounter += sub1 + 1;  // circle + center

      int indexCounter = 0;
      if (hasSide)
        indexCounter += subdivision * 6;
      if (hasTop)
        indexCounter += subdivision * 3;
      if (hasBottom)
        indexCounter += subdivision * 3;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      if (hasSide) {
        // define points around cylinder
        for (int i = 0; i < sub1; ++i) {
          const float alpha = (glm::pi<float>() * 2.0f * static_cast<float>(i)) / subdivision;
          const float x = glm::sin(alpha);
          const float y = glm::cos(alpha);
          const float d = (subdivision - static_cast<float>(i)) / subdivision;

          mesh->addCoord(glm::vec3(x, y, -h));
          mesh->addCoord(glm::vec3(x, y, h));
          mesh->addNormal(glm::vec3(x, y, 0.0f));
          mesh->addNormal(glm::vec3(x, y, 0.0f));
          mesh->addTexCoord(glm::vec2(d, 1.0f));
          mesh->addTexCoord(glm::vec2(d, 0.0f));
          mesh->addUnwrappedTexCoord(glm::vec2(d * 0.5f, 0.5f));
          mesh->addUnwrappedTexCoord(glm::vec2(d * 0.5f, 0.0f));
        }

        // connect points around cylinder
        int index = 0;
        for (int i = 0; i < subdivision; ++i) {
          mesh->addIndex(index);
          mesh->addIndex(index + 1);
          mesh->addIndex(index + 3);

          mesh->addIndex(index);
          mesh->addIndex(index + 3);
          mesh->addIndex(index + 2);
          index += 2;
        }
      }

      if (hasTop) {
        // center of cylinder top
        const int center = mesh->coords().size();
        mesh->addCoord(glm::vec3(0.0f, 0.0f, h));
        mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->addTexCoord(glm::vec2(0.5f, 0.5f));
        mesh->addUnwrappedTexCoord(glm::vec2(0.75f, 0.25f));

        // define cylinder top circle points
        for (int i = 0; i < sub1; ++i) {
          const float alpha = (glm::pi<float>() * 2.0f * static_cast<float>(i)) / subdivision;
          const float x = glm::sin(alpha);
          const float y = -glm::cos(alpha);

          mesh->addCoord(glm::vec3(x, -y, h));
          mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
          mesh->addTexCoord(glm::vec2(0.5f * x + 0.5f, 0.5f * y + 0.5f));
          mesh->addUnwrappedTexCoord(glm::vec2(0.25f * x + 0.75f, 0.25f * y + 0.25f));
        }

        // connect top circle points
        for (int i = 0, start = center + 1; i < subdivision; ++i, ++start) {
          mesh->addIndex(start + 1);
          mesh->addIndex(start);
          mesh->addIndex(center);
        }
      }

      if (hasBottom) {
        // center of cylinder bottom
        const int center = mesh->coords().size();
        mesh->addCoord(glm::vec3(0.0f, 0.0f, -h));
        mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
        mesh->addTexCoord(glm::vec2(0.5f, 0.5f));
        mesh->addUnwrappedTexCoord(glm::vec2(0.25f, 0.75f));

        // define cylinder bottom circle points
        for (int i = 0; i < sub1; ++i) {
          float alpha = (glm::pi<float>() * 2.0f * static_cast<float>(i)) / subdivision;
          float x = glm::sin(alpha);
          float y = -glm::cos(alpha);
          mesh->addCoord(glm::vec3(x, -y, -h));
          mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
          mesh->addTexCoord(glm::vec2(0.5f * x + 0.5f, -0.5f * y + 0.5f));
          mesh->addUnwrappedTexCoord(glm::vec2(0.25f * x + 0.25f, -0.25f * y + 0.75f));
        }

        // connect bottom circle points
        for (int i = 0, start = center + 1; i < subdivision; ++i, ++start) {
          mesh->addIndex(center);
          mesh->addIndex(start);
          mesh->addIndex(start + 1);
        }
      }
    }

    // bounding volumes
    const primitive::Box box;
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = box.computeBoundingSphere();
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createUnitElevationGrid(int dimensionX, int dimensionY, const float *heightData, float thickness,
                                                  bool outline) {
    // thickness only important for outline mesh
    float thickness2 = thickness;
    if (!outline)
      thickness2 = 0.0f;

    const std::array<float, 3> params = {{static_cast<float>(dimensionX), static_cast<float>(dimensionY), thickness2}};

    uint64_t meshHash = cache::sipHash13c(reinterpret_cast<const char *>(&params[0]), params.size() * sizeof(float));
    // cppcheck-suppress uninitvar
    meshHash ^= cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(heightData)),
                                  sizeof(float) * dimensionX * dimensionY);
    const cache::Key key(meshHash);

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    const float spacingX = 1.0f;
    const float spacingY = 1.0f;
    const int stepsX = dimensionX - 1;
    const int stepsY = dimensionY - 1;
    const float du = 1.0f / stepsX;
    const float dv = 1.0f / stepsY;

    mesh->estimateVertexCount(dimensionX * dimensionY);

    if (outline) {
      float minHeight = std::numeric_limits<float>::infinity();
      float maxHeight = -std::numeric_limits<float>::infinity();

      mesh->estimateIndexCount(stepsX * stepsY * 4);

      for (int yi = 0; yi < dimensionY; ++yi) {
        for (int xi = 0; xi < dimensionX; ++xi) {
          const float h = heightData[dimensionX * yi + xi];
          mesh->addCoord(glm::vec3(spacingX * xi, spacingY * yi, h));

          const int index = yi * dimensionX + xi;
          if (xi < dimensionX - 1) {
            mesh->addIndex(index);
            mesh->addIndex(index + 1);
          }

          if (yi < dimensionY - 1) {
            mesh->addIndex(index);
            mesh->addIndex(index + dimensionX);
          }

          if (h > maxHeight)
            maxHeight = h;

          if (h < minHeight)
            minHeight = h;
        }
      }

      if (minHeight != maxHeight) {
        const float bottom = minHeight - thickness;
        const float xMax = (dimensionX - 1) * spacingX;
        const float yMax = (dimensionY - 1) * spacingY;

        mesh->addCoord(glm::vec3(0, 0, bottom));
        mesh->addCoord(glm::vec3(0, 0, heightData[0]));
        mesh->addCoord(glm::vec3(xMax, 0, bottom));
        mesh->addCoord(glm::vec3(xMax, 0, heightData[dimensionX - 1]));
        mesh->addCoord(glm::vec3(xMax, yMax, bottom));
        mesh->addCoord(glm::vec3(xMax, yMax, heightData[(dimensionY - 1) * dimensionX + (dimensionX - 1)]));
        mesh->addCoord(glm::vec3(0, yMax, bottom));
        mesh->addCoord(glm::vec3(0, yMax, heightData[(dimensionY - 1) * dimensionX]));

        const int index = (dimensionX * dimensionY);
        mesh->addIndex(index);
        mesh->addIndex(index + 1);
        mesh->addIndex(index + 2);
        mesh->addIndex(index + 3);
        mesh->addIndex(index + 4);
        mesh->addIndex(index + 5);
        mesh->addIndex(index + 6);
        mesh->addIndex(index + 7);

        mesh->addIndex(index);
        mesh->addIndex(index + 2);
        mesh->addIndex(index + 2);
        mesh->addIndex(index + 4);
        mesh->addIndex(index + 4);
        mesh->addIndex(index + 6);
        mesh->addIndex(index + 6);
        mesh->addIndex(index);
      }

    } else {
      mesh->estimateIndexCount(stepsX * stepsY * 6);

      for (int yi = 0; yi < dimensionY; ++yi) {
        for (int xi = 0; xi < dimensionX; ++xi) {
          mesh->addCoord(
            glm::vec3(spacingX * xi, spacingY * (dimensionY - 1 - yi), heightData[dimensionX * (dimensionY - 1 - yi) + xi]));
          mesh->addTexCoord(glm::vec2(du * xi, dv * yi));
          mesh->addUnwrappedTexCoord(glm::vec2(du * xi, dv * yi));
        }
      }

      for (int yi = 0; yi < dimensionY; ++yi) {
        for (int xi = 0; xi < dimensionX; ++xi) {
          glm::vec3 normal, v0, v1, v2;
          // Average the normals of the 4 neighbouring triangles,
          // ignore triangles outside of the ElevationGrid
          normal = glm::vec3(0.0f);
          v0 = mesh->coords()[dimensionX * yi + xi];
          if (yi > 0 && xi > 0) {
            v1 = mesh->coords()[dimensionX * (yi - 1) + xi] - v0;
            v2 = mesh->coords()[dimensionX * yi + (xi - 1)] - v0;
            normal += glm::cross(v1, v2);
          }
          if (yi > 0 && xi < stepsX) {
            v1 = mesh->coords()[dimensionX * yi + (xi + 1)] - v0;
            v2 = mesh->coords()[dimensionX * (yi - 1) + xi] - v0;
            normal += glm::cross(v1, v2);
          }
          if (yi < stepsY && xi > 0) {
            v1 = mesh->coords()[dimensionX * yi + (xi - 1)] - v0;
            v2 = mesh->coords()[dimensionX * (yi + 1) + xi] - v0;
            normal += glm::cross(v1, v2);
          }
          if (yi < stepsY && xi < stepsX) {
            v1 = mesh->coords()[dimensionX * (yi + 1) + xi] - v0;
            v2 = mesh->coords()[dimensionX * yi + (xi + 1)] - v0;
            normal += glm::cross(v1, v2);
          }

          mesh->addNormal(glm::normalize(normal));
        }
      }

      for (int yi = 0; yi < stepsY; ++yi) {
        for (int xi = 0; xi < stepsX; ++xi) {
          // first triangle
          mesh->addIndex(dimensionX * yi + xi);
          mesh->addIndex(dimensionX * (yi + 1) + xi);
          mesh->addIndex(dimensionX * yi + (xi + 1));
          // second triangle
          mesh->addIndex(dimensionX * yi + (xi + 1));
          mesh->addIndex(dimensionX * (yi + 1) + xi);
          mesh->addIndex(dimensionX * (yi + 1) + (xi + 1));
        }
      }
    }

    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createUnitRectangle(bool outline) {
    StaticMesh *mesh;
    if (outline) {
      const cache::Key key(cache::sipHash13c("RectangleOutline", strlen("RectangleOutline")));

      if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
        return mesh;

      mesh->estimateVertexCount(4);
      mesh->estimateIndexCount(8);

      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.0f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.0f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.0f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.0f));

      mesh->addIndex(0);
      mesh->addIndex(1);
      mesh->addIndex(1);
      mesh->addIndex(2);
      mesh->addIndex(2);
      mesh->addIndex(3);
      mesh->addIndex(3);
      mesh->addIndex(0);
    } else {
      const cache::Key key(cache::sipHash13c("Rectangle", strlen("Rectangle")));

      if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
        return mesh;

      mesh->estimateVertexCount(4);
      mesh->estimateIndexCount(6);

      mesh->addCoord(glm::vec3(-0.5f, -0.5f, 0.0f));
      mesh->addCoord(glm::vec3(0.5f, -0.5f, 0.0f));
      mesh->addCoord(glm::vec3(0.5f, 0.5f, 0.0f));
      mesh->addCoord(glm::vec3(-0.5f, 0.5f, 0.0f));

      for (int j = 0; j < 4; ++j)
        mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));

      mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addTexCoord(glm::vec2(0.0f, 0.0f));

      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 1.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 0.0f));
      mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 0.0f));

      mesh->addIndex(0);
      mesh->addIndex(1);
      mesh->addIndex(2);
      mesh->addIndex(0);
      mesh->addIndex(2);
      mesh->addIndex(3);
    }

    // bounding volumes
    const primitive::Box box;
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = box.computeBoundingSphere();
      mesh->mCacheData->mAabb =
        primitive::Aabb(glm::vec3(-0.5f, -0.5f, -glm::epsilon<float>()), glm::vec3(0.5f, 0.5f, glm::epsilon<float>()));
    }

    mesh->setup();

    return mesh;
  }

  static float cartesianCoordinatesToPolarAngle(float x, float y) {
    float phi = glm::atan(y, x);
    if (phi < 0)
      return 2 * glm::pi<float>() + phi;

    return phi;
  };

  static void subdividePolyhedronFace(StaticMesh *mesh, const glm::vec3 *v1, const glm::vec3 *v2, const glm::vec3 *v3,
                                      int level, bool outline) {
    if (level == 0) {
      // get the x-axis texture coordinate of each vertex
      float a1 = cartesianCoordinatesToPolarAngle(v1->y, -v1->x) * 0.5f * glm::one_over_pi<float>();
      float a2 = cartesianCoordinatesToPolarAngle(v2->y, -v2->x) * 0.5f * glm::one_over_pi<float>();
      float a3 = cartesianCoordinatesToPolarAngle(v3->y, -v3->x) * 0.5f * glm::one_over_pi<float>();

      // fix the bottom and the top of the texture coordinates
      if (v1->x == 0.0f && v1->y == 0.0f) {  // if v1 corresponds to a summit
        if (a2 == 0.0f && a3 > 0.8f)
          a3 -= 1.0f;
        if (a3 == 0.0f && a2 > 0.8f)
          a2 -= 1.0f;
        a1 = (a2 + a3) / 2.0;                       // compute the mean of the two other angles
      } else if (v2->x == 0.0f && v2->y == 0.0f) {  // if v2 corresponds to a summit
        if (a1 == 0.0f && a3 > 0.8f)
          a3 -= 1.0f;
        if (a3 == 0.0f && a1 > 0.8f)
          a1 -= 1.0f;
        a2 = (a1 + a3) / 2.0;                       // compute the mean of the two other angles
      } else if (v3->x == 0.0f && v3->y == 0.0f) {  // if v3 corresponds to a summit
        if (a1 == 0.0f && a2 > 0.8f)
          a2 -= 1.0f;
        if (a2 == 0.0f && a1 > 0.8f)
          a1 -= 1.0f;
        a3 = (a1 + a2) / 2.0;  // compute the mean of the two other angles
        // fix the joint of the texture coordinates
      } else if (a1 == 0.0f) {  // the first vertex is defined on the joint
        if (a2 > 0.8f && a3 > 0.8f)
          a1 = 1.0f;
        else if (a2 > 0.8f) {
          a1 = 1.0f;
          a3 += 1.0f;
        } else if (a3 > 0.8f)
          a3 -= 1.0f;
      } else if (a2 == 0.0f) {  // the second vertex is defined on the joint
        if (a1 > 0.8f && a3 > 0.8f)
          a2 = 1.0f;
        else if (a1 > 0.8f)
          a1 -= 1.0f;
        else if (a3 > 0.8f) {
          a2 = 1.0f;
          a1 += 1.0f;
        }
      } else if (a3 == 0.0f) {  // the third vertex is defined on the joint
        if (a1 > 0.8f && a2 > 0.8f)
          a3 = 1.0f;
        else if (a2 > 0.8f) {
          a3 = 1.0f;
          a1 += 1.0f;
        } else if (a1 > 0.8f)
          a1 -= 1.0f;
      }

      mesh->addCoord(glm::vec3(v3->x, v3->y, v3->z));
      mesh->addCoord(glm::vec3(v2->x, v2->y, v2->z));
      mesh->addCoord(glm::vec3(v1->x, v1->y, v1->z));

      if (!outline) {
        mesh->addNormal(glm::vec3(v3->x, v3->y, v3->z));
        mesh->addNormal(glm::vec3(v2->x, v2->y, v2->z));
        mesh->addNormal(glm::vec3(v1->x, v1->y, v1->z));

        const glm::vec2 uv1(a1, 0.5f - glm::asin(v1->z) * glm::one_over_pi<float>());
        const glm::vec2 uv2(a2, 0.5f - glm::asin(v2->z) * glm::one_over_pi<float>());
        const glm::vec2 uv3(a3, 0.5f - glm::asin(v3->z) * glm::one_over_pi<float>());
        mesh->addTexCoord(uv3);
        mesh->addTexCoord(uv2);
        mesh->addTexCoord(uv1);
        mesh->addUnwrappedTexCoord(uv3);
        mesh->addUnwrappedTexCoord(uv2);
        mesh->addUnwrappedTexCoord(uv1);
      }
    } else {
      const glm::vec3 v12 = glm::normalize(*v1 + *v2);
      const glm::vec3 v23 = glm::normalize(*v2 + *v3);
      const glm::vec3 v31 = glm::normalize(*v3 + *v1);

      subdividePolyhedronFace(mesh, v1, &v12, &v31, level - 1, outline);
      subdividePolyhedronFace(mesh, v2, &v23, &v12, level - 1, outline);
      subdividePolyhedronFace(mesh, v3, &v31, &v23, level - 1, outline);
      subdividePolyhedronFace(mesh, &v12, &v23, &v31, level - 1, outline);
    }
  };

  StaticMesh *StaticMesh::createUnitIcosphere(int subdivision, bool outline) {
    char uniqueName[25];
    if (outline)
      snprintf(uniqueName, sizeof(uniqueName), "IcosphereOutline%d", subdivision);
    else
      snprintf(uniqueName, sizeof(uniqueName), "Icosphere%d", subdivision);
    const cache::Key key(cache::sipHash13c(uniqueName, strlen(uniqueName)));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    static const float A = 0.525731112119133606f;
    static const float B = 0.850650808352039932f;

    static const glm::vec3 gVertices[12] = {glm::vec3(-A, -B, 0.0f), glm::vec3(A, -B, 0.0f), glm::vec3(-A, B, 0.0f),
                                            glm::vec3(A, B, 0.0f),   glm::vec3(0.0f, -A, B), glm::vec3(0.0f, A, B),
                                            glm::vec3(0.0f, -A, -B), glm::vec3(0.0f, A, -B), glm::vec3(B, 0.0f, A),
                                            glm::vec3(-B, 0.0f, A),  glm::vec3(B, 0.0f, -A), glm::vec3(-B, 0.0f, -A)};

    static const glm::uvec3 gIndices[20] = {
      glm::uvec3(0, 4, 1),  glm::uvec3(0, 9, 4),  glm::uvec3(9, 5, 4),  glm::uvec3(4, 5, 8),  glm::uvec3(4, 8, 1),
      glm::uvec3(8, 10, 1), glm::uvec3(8, 3, 10), glm::uvec3(5, 3, 8),  glm::uvec3(5, 2, 3),  glm::uvec3(2, 7, 3),
      glm::uvec3(7, 10, 3), glm::uvec3(7, 6, 10), glm::uvec3(7, 11, 6), glm::uvec3(11, 0, 6), glm::uvec3(0, 1, 6),
      glm::uvec3(6, 1, 10), glm::uvec3(9, 0, 11), glm::uvec3(9, 11, 2), glm::uvec3(9, 2, 5),  glm::uvec3(7, 2, 11)};

    int vertexCounter;
    if (subdivision == 0)
      vertexCounter = 60;
    else
      vertexCounter = 60 * (4 << (2 * (subdivision - 1)));

    mesh->estimateVertexCount(vertexCounter);
    mesh->estimateIndexCount(vertexCounter);

    // iterate over all faces and apply a subdivison with the given value
    for (int i = 0; i < 20; ++i)
      subdividePolyhedronFace(mesh, &gVertices[gIndices[i].x], &gVertices[gIndices[i].y], &gVertices[gIndices[i].z],
                              subdivision, outline);

    for (int i = 0; i < vertexCounter; ++i)
      mesh->addIndex(i);

    // bounding volumes
    const primitive::Rectangle rect;
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::Sphere(gVec3Zeros, 1.0f);
      mesh->mCacheData->mAabb = primitive::Aabb(glm::vec3(-1.0f), glm::vec3(1.0f));
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createUnitUVSphere(int subdivision, bool outline) {
    char uniqueName[24];
    if (outline)
      snprintf(uniqueName, sizeof(uniqueName), "UVSphereOutline%d", subdivision);
    else
      snprintf(uniqueName, sizeof(uniqueName), "UVSphere%d", subdivision);
    const cache::Key key(cache::sipHash13c(uniqueName, strlen(uniqueName)));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    const int rowSize = subdivision + 1;
    const int vertexCounter = rowSize * rowSize;
    mesh->estimateVertexCount(vertexCounter);
    mesh->estimateIndexCount(3 * (vertexCounter - 1));

    int r, s;
    const float latitudeUnitAngle = glm::pi<float>() / subdivision;
    const float longitudeUnitAngle = 2.0f * glm::pi<float>() / subdivision;
    int index = 0;
    int **indicesGrid = new int *[rowSize];

    // vertices
    for (r = 0; r <= subdivision; ++r) {  // rings/latitude
      // special case for the poles
      const float uOffset = (r == 0) ? 0.5f / subdivision : ((r == subdivision) ? -0.5f / subdivision : 0.0f);
      const float theta = (float)r * latitudeUnitAngle;
      const float sinTheta = glm::sin(theta);
      const float cosTheta = glm::cos(theta);
      int *indicesRow = new int[rowSize];
      for (s = 0; s <= subdivision; ++s) {  // segments/longitude
        glm::vec3 vertex;
        const float phi = ((float)s) * longitudeUnitAngle + glm::half_pi<float>();
        vertex = glm::vec3(glm::cos(phi) * sinTheta, glm::sin(phi) * sinTheta, cosTheta);
        mesh->addCoord(vertex);
        if (!outline) {
          mesh->addNormal(vertex);

          glm::vec2 uv((float)s / subdivision + uOffset, (float)r / subdivision);
          mesh->addTexCoord(uv);
          mesh->addUnwrappedTexCoord(uv);
        }

        indicesRow[s] = index;
        index++;
      }
      indicesGrid[r] = indicesRow;
    }

    // indices
    if (outline) {
      const int lastRingIndex = vertexCounter - subdivision - 1;
      for (s = 0; s < subdivision; ++s) {
        // top
        mesh->addIndex(0);
        mesh->addIndex(subdivision + s);
        // bottom
        mesh->addIndex(lastRingIndex);
        mesh->addIndex(lastRingIndex - s - 1);
      }

      // side
      for (r = 1; r < (subdivision - 1); ++r) {
        for (s = 0; s < subdivision; ++s) {
          int a = indicesGrid[r][s + 1];
          int b = indicesGrid[r][s];
          int c = indicesGrid[r + 1][s];
          int d = indicesGrid[r + 1][s + 1];

          mesh->addIndex(a);
          mesh->addIndex(b);
          mesh->addIndex(b);
          mesh->addIndex(c);
          mesh->addIndex(c);
          mesh->addIndex(d);
          mesh->addIndex(b);
          mesh->addIndex(d);
        }
      }
    } else {
      for (r = 0; r < subdivision; ++r) {
        for (s = 0; s < subdivision; ++s) {
          int a = indicesGrid[r][s + 1];
          int b = indicesGrid[r][s];
          int c = indicesGrid[r + 1][s];
          int d = indicesGrid[r + 1][s + 1];

          if (r != 0) {
            mesh->addIndex(a);
            mesh->addIndex(b);
            mesh->addIndex(d);
          }
          if (r != subdivision - 1) {
            mesh->addIndex(b);
            mesh->addIndex(c);
            mesh->addIndex(d);
          }
        }
      }
    }

    // cleanup
    for (r = 0; r <= subdivision; ++r)
      delete[] indicesGrid[r];
    delete[] indicesGrid;

    // bounding volumes
    const primitive::Rectangle rect;
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::Sphere(gVec3Zeros, 1.0f);
      mesh->mCacheData->mAabb = primitive::Aabb(glm::vec3(-1.0f), glm::vec3(1.0f));
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createCapsule(int subdivision, float radius, float height, bool hasSide, bool hasTop, bool hasBottom,
                                        bool outline) {
    // hash all params with in a single call to sipHash13c
    constexpr int paramsSize = sizeof(int) + sizeof(float) * 2 + sizeof(bool) * 4;
    char params[paramsSize];
    char *dest = &params[0];

    memcpy(dest, reinterpret_cast<const void *>(&subdivision), sizeof(int));
    dest += sizeof(int);
    memcpy(dest, reinterpret_cast<const void *>(&height), sizeof(float));
    dest += sizeof(float);
    memcpy(dest, reinterpret_cast<const void *>(&radius), sizeof(float));
    dest += sizeof(float);
    memcpy(dest, reinterpret_cast<const void *>(&hasSide), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&hasTop), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&hasBottom), sizeof(bool));
    dest += sizeof(bool);
    memcpy(dest, reinterpret_cast<const void *>(&outline), sizeof(bool));

    const cache::Key key(cache::sipHash13c(&params[0], paramsSize));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    const int sub1 = subdivision + 1;
    const int sub4 = subdivision / 4;
    const int sub5 = sub4 + 1;
    const float halfHeight = 0.5f * height;
    const float top = 2.0f / 3.0f;
    const float bottom = 1.0f / 3.0f;

    if (outline) {
      int vertexCounter = 2 + subdivision * 2;
      vertexCounter += subdivision * 2 * (sub4 - 1);

      int indexCounter = 0;
      // Top & bottom
      indexCounter += subdivision * 2 + ((sub4 - 1) * subdivision * 2);
      indexCounter *= 2;
      // Sides
      indexCounter += subdivision * 2 + subdivision * 4;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      const float factor = 2.0f * glm::pi<float>() / subdivision;
      std::vector<float> x(sub1);
      std::vector<float> y(sub1);
      for (int i = 0; i < sub1; ++i) {
        const float alpha = factor * i;
        x[i] = glm::sin(alpha);
        y[i] = glm::cos(alpha);
      }

      for (int i = 0; i < sub1; ++i) {
        const float rx = radius * x[i];
        const float ry = radius * y[i];
        mesh->addCoord(glm::vec3(rx, ry, -halfHeight));
        mesh->addCoord(glm::vec3(rx, ry, halfHeight));
      }

      for (int i = 0, start = 0; i < subdivision; ++i, start += 2) {
        mesh->addIndex(start);
        mesh->addIndex(start + 1);
        mesh->addIndex(start);
        mesh->addIndex(start + 2);
        mesh->addIndex(start + 1);
        mesh->addIndex(start + 3);
      }

      // compute quarter of circle profile for capsule top
      std::vector<float> ay(sub5);
      std::vector<float> ar(sub5);
      const float factor4 = 0.5f * glm::pi<float>() / sub4;
      for (int i = 0; i < sub5; ++i) {
        float alpha = factor4 * i;
        ay[i] = halfHeight + radius * glm::sin(alpha);
        ar[i] = -radius * glm::cos(alpha);
      }

      // compute coordinates for the capsule top cap
      // points
      // format: [horizontal_index][vertical_index][xyz_index]
      float ***v = new float **[sub1];
      for (int i = 0; i < sub1; ++i) {
        float beta = factor * i;
        v[i] = new float *[sub5];
        for (int j = 0; j < sub5; ++j) {
          // compute vertices
          float *cv = new float[3];
          cv[0] = ar[j] * glm::sin(beta);
          cv[1] = ar[j] * -glm::cos(beta);
          cv[2] = ay[j];
          v[i][j] = cv;
        }
      }

      // Top
      int start = mesh->coords().size();
      for (int i = 0; i < sub1; ++i) {
        for (int j = 0; j < sub5; ++j) {
          float *cv = v[i][j];
          mesh->addCoord(glm::vec3(cv[0], cv[1], cv[2]));
        }
      }

      int **idx = new int *[sub1];
      for (int i = 0; i < sub1; ++i) {
        idx[i] = new int[sub5];
        for (int j = 0; j < sub5; ++j)
          idx[i][j] = start++;
      }

      for (int i = 0; i < subdivision; ++i) {  // horizontally
        for (int j = 0; j < sub4; ++j) {       // vertically
          mesh->addIndex(idx[i][j]);
          mesh->addIndex(idx[i + 1][j]);

          mesh->addIndex(idx[i][j]);
          mesh->addIndex(idx[i][j + 1]);
        }
      }

      // Bottom
      start = mesh->coords().size();
      for (int i = 0; i < sub1; ++i) {
        for (int j = 0; j < sub5; ++j) {
          float *cv = v[i][j];
          mesh->addCoord(glm::vec3(cv[0], cv[1], -cv[2]));
        }
      }

      for (int i = 0; i < sub1; ++i) {
        delete[] idx[i];

        for (int j = 0; j < sub5; ++j)
          delete[] v[i][j];

        delete[] v[i];
      }
      delete[] v;
      delete[] idx;

      idx = new int *[sub1];
      for (int i = 0; i < sub1; ++i) {
        idx[i] = new int[sub5];
        for (int j = 0; j < sub5; ++j)
          idx[i][j] = start++;
      }

      for (int i = 0; i < subdivision; ++i) {  // horizontally
        for (int j = 0; j < sub4; ++j) {       // vertically
          mesh->addIndex(idx[i][j]);
          mesh->addIndex(idx[i + 1][j]);

          mesh->addIndex(idx[i][j]);
          mesh->addIndex(idx[i][j + 1]);
        }
      }

      for (int i = 0; i < sub1; ++i)
        delete[] idx[i];
      delete[] idx;

    } else {
      int vertexCounter = 0;
      if (hasTop || hasBottom) {
        vertexCounter += sub1 * sub5;
        if (hasBottom && hasTop)
          vertexCounter *= 2;
      }
      if (hasSide)
        vertexCounter += 2 * sub1;

      int indexCounter = 0;
      if (hasTop || hasBottom) {
        indexCounter += subdivision * ((sub4 - 1) * 6 + 3);
        if (hasBottom && hasTop)
          indexCounter *= 2;
      }
      if (hasSide)
        indexCounter += 6 * subdivision;

      mesh->estimateVertexCount(vertexCounter);
      mesh->estimateIndexCount(indexCounter);

      if (hasSide) {
        // define points around capsule
        const float factor = 2.0f * glm::pi<float>() / subdivision;
        std::vector<float> x(sub1);
        std::vector<float> y(sub1);
        for (int i = 0; i < sub1; ++i) {
          const float alpha = factor * i;
          x[i] = glm::sin(alpha);
          y[i] = glm::cos(alpha);
        }

        for (int i = 0; i < sub1; ++i) {
          const float rx = radius * x[i];
          const float ry = radius * y[i];
          mesh->addCoord(glm::vec3(rx, ry, -halfHeight));
          mesh->addCoord(glm::vec3(rx, ry, halfHeight));
        }

        for (int i = 0; i < sub1; ++i) {
          mesh->addNormal(glm::vec3(x[i], y[i], 0.0f));
          mesh->addNormal(glm::vec3(x[i], y[i], 0.0f));
        }

        for (int i = 0; i < sub1; ++i) {
          const float d = (float)(subdivision - i) / subdivision;
          mesh->addTexCoord(glm::vec2(d, top));
          mesh->addTexCoord(glm::vec2(d, bottom));
          mesh->addUnwrappedTexCoord(glm::vec2(d, top));
          mesh->addUnwrappedTexCoord(glm::vec2(d, bottom));
        }

        for (int i = 0, start = 0; i < subdivision; ++i, start += 2) {
          mesh->addIndex(start);
          mesh->addIndex(start + 1);
          mesh->addIndex(start + 3);

          mesh->addIndex(start);
          mesh->addIndex(start + 3);
          mesh->addIndex(start + 2);
        }
      }

      if (hasTop || hasBottom) {
        // compute quarter of circle profile for capsule top
        std::vector<float> ay(sub5);
        std::vector<float> ar(sub5);
        const float factor4 = 0.5f * glm::pi<float>() / sub4;
        for (int i = 0; i < sub5; ++i) {
          float alpha = factor4 * i;
          ay[i] = halfHeight + radius * glm::sin(alpha);
          ar[i] = -radius * glm::cos(alpha);
        }

        // compute coordinates for the capsule top cap
        // points and normal coordinates of top capsule
        // format: [horizontal_index][vertical_index][xyz_index]
        float ***v = new float **[sub1];
        float ***n = new float **[sub1];
        float ***t = new float **[sub1];
        const float invSub = 1.0f / subdivision;
        const float inv4 = 1.0f / (sub4 * 3.0f);
        const float factor = 2.0f * glm::pi<float>() / subdivision;
        for (int i = 0; i < sub1; ++i) {
          float beta = factor * i;
          float d1 = invSub * i;
          v[i] = new float *[sub5];
          n[i] = new float *[sub5];
          t[i] = new float *[sub5];
          for (int j = 0; j < sub5; ++j) {
            const float d2 = (sub4 - j) * inv4;

            // compute vertices
            float *cv = new float[3];
            cv[0] = ar[j] * glm::sin(beta);
            cv[1] = -ar[j] * glm::cos(beta);
            cv[2] = ay[j];
            v[i][j] = cv;

            // compute normals
            float *cn = new float[3];
            glm::vec3 cnvec3 = glm::normalize(glm::vec3(cv[0], cv[1], cv[2] - halfHeight));
            cn[0] = cnvec3[0];
            cn[1] = cnvec3[1];
            cn[2] = cnvec3[2];
            n[i][j] = cn;

            // compute texture coordinates
            // the condition is to avoid sawtooth effect on top/bottom row
            float *ct = new float[2];
            ct[0] = j < sub4 ? d1 : (i + 0.5f) / subdivision;
            ct[1] = d2;
            t[i][j] = ct;
          }
        }

        if (hasTop) {
          // define points for top part
          int start = mesh->coords().size();
          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *cv = v[i][j];
              mesh->addCoord(glm::vec3(cv[0], cv[1], cv[2]));
            }
          }
          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *cn = n[i][j];
              mesh->addNormal(glm::vec3(cn[0], cn[1], cn[2]));
            }
          }
          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *ct = t[i][j];
              mesh->addTexCoord(glm::vec2(ct[0], ct[1]));
              mesh->addUnwrappedTexCoord(glm::vec2(ct[0], ct[1]));
            }
          }

          int **idx = new int *[sub1];
          for (int i = 0; i < sub1; ++i) {
            idx[i] = new int[sub5];
            for (int j = 0; j < sub5; ++j)
              idx[i][j] = start++;
          }

          // create faces
          for (int i = 0; i < subdivision; ++i) {  // horizontally
            for (int j = 0; j < sub4; ++j) {       // vertically
              if (j < sub4 - 1) {
                mesh->addIndex(idx[i][j]);
                mesh->addIndex(idx[i + 1][j]);
                mesh->addIndex(idx[i + 1][j + 1]);

                mesh->addIndex(idx[i][j]);
                mesh->addIndex(idx[i + 1][j + 1]);
                mesh->addIndex(idx[i][j + 1]);
              } else {
                // top row needs triangles not quads
                mesh->addIndex(idx[i][j]);
                mesh->addIndex(idx[i + 1][j]);
                mesh->addIndex(idx[i][j + 1]);
              }
            }
          }

          for (int i = 0; i < sub1; ++i)
            delete[] idx[i];
          delete[] idx;
        }

        if (hasBottom) {
          // define points for bottom part
          // we reuse the points of the top part and negate the y-components
          int start = mesh->coords().size();
          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *cv = v[i][j];
              mesh->addCoord(glm::vec3(cv[0], cv[1], -cv[2]));
            }
          }

          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *cn = n[i][j];
              mesh->addNormal(glm::vec3(cn[0], cn[1], -cn[2]));
            }
          }

          for (int i = 0; i < sub1; ++i) {
            for (int j = 0; j < sub5; ++j) {
              float *ct = t[i][j];
              mesh->addTexCoord(glm::vec2(ct[0], 1.0 - ct[1]));
              mesh->addUnwrappedTexCoord(glm::vec2(ct[0], 1.0 - ct[1]));
            }
          }

          int **idx = new int *[sub1];
          for (int i = 0; i < sub1; ++i) {
            idx[i] = new int[sub5];
            for (int j = 0; j < sub5; ++j)
              idx[i][j] = start++;
          }
          // create faces
          for (int i = 0; i < subdivision; ++i) {  // horizontally
            for (int j = 0; j < sub4; ++j) {       // vertically
              if (j < sub4 - 1) {
                mesh->addIndex(idx[i][j + 1]);
                mesh->addIndex(idx[i + 1][j + 1]);
                mesh->addIndex(idx[i + 1][j]);

                mesh->addIndex(idx[i][j + 1]);
                mesh->addIndex(idx[i + 1][j]);
                mesh->addIndex(idx[i][j]);
              } else {
                // bottow row needs triangles not quads
                mesh->addIndex(idx[i][j + 1]);
                mesh->addIndex(idx[i + 1][j]);
                mesh->addIndex(idx[i][j]);
              }
            }
          }

          for (int i = 0; i < sub1; ++i)
            delete[] idx[i];
          delete[] idx;
        }

        // cleanup
        for (int i = 0; i < sub1; ++i) {
          for (int j = 0; j < sub5; ++j) {
            delete[] v[i][j];
            delete[] n[i][j];
            delete[] t[i][j];
          }
          delete[] v[i];
          delete[] n[i];
          delete[] t[i];
        }
        delete[] v;
        delete[] n;
        delete[] t;
      }
    }

    // bounding volumes
    const primitive::Capsule capsule(gVec3Zeros, radius, height, hasSide, hasTop, hasBottom);
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = capsule.computeBoundingSphere();
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createQuad() {
    const cache::Key key(cache::sipHash13c("Quad", strlen("Quad")));

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    mesh->estimateVertexCount(4);
    mesh->estimateIndexCount(6);

    mesh->addCoord(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->addCoord(glm::vec3(0.0f, 0.0f, 0.0f));
    mesh->addCoord(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->addCoord(glm::vec3(1.0f, 1.0f, 0.0f));

    mesh->addTexCoord(glm::vec2(0.0f, 1.0f));
    mesh->addTexCoord(glm::vec2(0.0f, 0.0f));
    mesh->addTexCoord(glm::vec2(1.0f, 0.0f));
    mesh->addTexCoord(glm::vec2(1.0f, 1.0f));

    mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 1.0f));
    mesh->addUnwrappedTexCoord(glm::vec2(0.0f, 0.0f));
    mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 0.0f));
    mesh->addUnwrappedTexCoord(glm::vec2(1.0f, 1.0f));

    // bottom-left triangle
    mesh->addIndex(0);
    mesh->addIndex(1);
    mesh->addIndex(2);

    // top-right triangle
    mesh->addIndex(2);
    mesh->addIndex(3);
    mesh->addIndex(0);

    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createLineSet(int coordCount, const float *coordData, const float *colorData) {
    uint64_t meshHash = cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(coordData)),
                                          sizeof(glm::vec3) * coordCount);
    if (colorData) {
      // cppcheck-suppress uninitvar
      meshHash ^= cache::sipHash13c("colorData", 9);
      meshHash ^= cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(colorData)),
                                    sizeof(glm::vec3) * coordCount);
    }

    const cache::Key key(meshHash);

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    std::vector<glm::vec3> &coords = mesh->coords();
    coords.resize(coordCount);

    std::memcpy(reinterpret_cast<void *>(coords.data()), reinterpret_cast<const void *>(coordData),
                coordCount * sizeof(glm::vec3));

    mesh->estimateIndexCount(coordCount);
    for (int i = 0; i < coordCount; ++i)
      mesh->addIndex(i);

    // (Optional) Color per vertex
    if (colorData) {
      std::vector<glm::vec3> &colors = mesh->colors();
      colors.resize(coordCount);
      std::memcpy(reinterpret_cast<void *>(colors.data()), reinterpret_cast<const void *>(colorData),
                  coordCount * sizeof(glm::vec3));
    }

    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::createPointSet(int coordCount, const float *coordData, const float *colorData) {
    uint64_t meshHash = cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(coordData)),
                                          sizeof(glm::vec3) * coordCount);
    if (colorData) {
      // cppcheck-suppress uninitvar
      meshHash ^= cache::sipHash13c("colorData", 9);
      meshHash ^= cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(colorData)),
                                    sizeof(glm::vec3) * coordCount);
    }

    const cache::Key key(meshHash);

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    std::vector<glm::vec3> &coords = mesh->coords();
    coords.resize(coordCount);

    std::memcpy(reinterpret_cast<void *>(coords.data()), reinterpret_cast<const void *>(coordData),
                coordCount * sizeof(glm::vec3));

    mesh->estimateIndexCount(coordCount);
    for (int i = 0; i < coordCount; ++i)
      mesh->addIndex(i);

    // (Optional) Color per vertex
    if (colorData) {
      std::vector<glm::vec3> &colors = mesh->colors();
      colors.resize(coordCount);
      std::memcpy(reinterpret_cast<void *>(colors.data()), reinterpret_cast<const void *>(colorData),
                  coordCount * sizeof(glm::vec3));
    }

    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  StaticMesh *StaticMesh::preCreatePointSet(int coordCount, const float *coordData, const float *colorData, float *&coordPtr, float *&colorPtr) {
    uint64_t hash = cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(coordData)), 4);
    if (colorData) {
      // cppcheck-suppress uninitvar
      hash ^= cache::sipHash13c("colorData", 9);
      hash ^= cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(colorData)), 4);
    }

    const cache::Key key(hash);

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    std::vector<glm::vec3> &coords = mesh->coords();
    coords.resize(coordCount);

	coordPtr = reinterpret_cast<float *>(coords.data());

    mesh->estimateIndexCount(coordCount);
    for (int i = 0; i < coordCount; ++i)
      mesh->addIndex(i);

    // (Optional) Color per vertex
    if (colorData) {
      std::vector<glm::vec3> &colors = mesh->colors();
      colors.resize(coordCount);
	  colorPtr = reinterpret_cast<float *>(colors.data());
    }

    return mesh;
  }

  void StaticMesh::postCreatePointSet(StaticMesh *mesh) {
    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();
  }

  StaticMesh *StaticMesh::createTriangleMesh(int coordCount, int indexCount, const float *coordData, const float *normalData,
                                             const float *texCoordData, const float *unwrappedTexCoordData,
                                             const unsigned int *indexData, bool outline) {
    uint64_t meshHash = cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(coordData)),
                                          sizeof(glm::vec3) * coordCount);
    // cppcheck-suppress uninitvar
    meshHash ^= cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(normalData)),
                                  sizeof(glm::vec3) * coordCount);
    uint64_t texCoordDataHash = 0;
    if (texCoordData) {
      texCoordDataHash = cache::sipHash13c(reinterpret_cast<const char *>(reinterpret_cast<const void *>(texCoordData)),
                                           sizeof(glm::vec2) * coordCount);
      meshHash ^= texCoordDataHash;
    }
    if (unwrappedTexCoordData) {
      uint64_t unwrappedTexCoordDataHash = cache::sipHash13c(
        reinterpret_cast<const char *>(reinterpret_cast<const void *>(unwrappedTexCoordData)), sizeof(glm::vec2) * coordCount);
      if (!texCoordData || (texCoordDataHash != unwrappedTexCoordDataHash))
        meshHash ^= unwrappedTexCoordDataHash;
    }
    meshHash ^= cache::sipHash13c(reinterpret_cast<const char *>(indexData), sizeof(unsigned int) * indexCount);

    if (outline)
      meshHash = ~meshHash;

    const cache::Key key(meshHash);

    StaticMesh *mesh;
    if (StaticMesh::createOrRetrieveFromCache(&mesh, key))
      return mesh;

    std::vector<glm::vec3> &coords = mesh->coords();
    coords.resize(coordCount);
    std::memcpy(reinterpret_cast<void *>(coords.data()), reinterpret_cast<const void *>(coordData),
                coordCount * sizeof(glm::vec3));

    if (outline) {
      mesh->estimateIndexCount(indexCount * 2);

      for (int i = 0; i < indexCount - 2; i += 3) {
        mesh->addIndex(indexData[i]);
        mesh->addIndex(indexData[i + 1]);

        mesh->addIndex(indexData[i + 1]);
        mesh->addIndex(indexData[i + 2]);

        mesh->addIndex(indexData[i + 2]);
        mesh->addIndex(indexData[i]);
      }
    } else {
      std::vector<glm::vec3> &normals = mesh->normals();
      normals.resize(coordCount);
      std::memcpy(reinterpret_cast<void *>(normals.data()), reinterpret_cast<const void *>(normalData),
                  coordCount * sizeof(glm::vec3));

      if (texCoordData) {
        std::vector<glm::vec2> &texCoords = mesh->texCoords();
        texCoords.resize(coordCount);
        std::memcpy(reinterpret_cast<void *>(texCoords.data()), reinterpret_cast<const void *>(texCoordData),
                    coordCount * sizeof(glm::vec2));
      }

      if (unwrappedTexCoordData) {
        std::vector<glm::vec2> &unwrappedTexCoords = mesh->unwrappedTexCoords();
        unwrappedTexCoords.resize(coordCount);
        std::memcpy(reinterpret_cast<void *>(unwrappedTexCoords.data()), reinterpret_cast<const void *>(unwrappedTexCoordData),
                    coordCount * sizeof(glm::vec2));
      }

      std::vector<unsigned int> &indices = mesh->indices();
      indices.resize(indexCount);
      std::memcpy(reinterpret_cast<void *>(indices.data()), reinterpret_cast<const void *>(indexData),
                  indexCount * sizeof(unsigned int));
    }

    // bounding volumes
    if (mesh->mCacheData) {
      mesh->mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(mesh->coords());
      mesh->mCacheData->mAabb = primitive::Aabb(mesh->coords());
    }

    mesh->setup();

    return mesh;
  }

  size_t StaticMesh::cachedItemCount() {
    size_t nonPersistentCount = 0;
    for (std::pair<cache::Key, cache::MeshData> cachedMesh : cCache)
      if (!cachedMesh.second.mIsCachePersistent)
        ++nonPersistentCount;
    return nonPersistentCount;
  }

  void StaticMesh::setCachePersistency(bool persistent) {
    mIsCachePersistent = persistent;
    if (mCacheData)
      mCacheData->mIsCachePersistent = persistent;
  }

  void StaticMesh::bind() {
    if (!mCacheData)
      return;

    if (mCacheData->mGlNameVertexArrayObject)
      glstate::bindVertexArrayObject(mCacheData->mGlNameVertexArrayObject);

    // Indices should be part VAO state, but this doesn't seem to be the case for all
    // drivers, so we need to bind the buffer manually to be sure
    if (mCacheData->mGlNameBufferIndices)
      glstate::bindElementArrayBuffer(mCacheData->mGlNameBufferIndices);
  }

  void StaticMesh::release() {
    if (mCacheData) {
      if (mCacheData->mGlNameVertexArrayObject)
        glstate::releaseVertexArrayObject(mCacheData->mGlNameVertexArrayObject);
      if (mCacheData->mGlNameBufferIndices)
        glstate::releaseElementArrayBuffer(mCacheData->mGlNameBufferIndices);
    }
  }

  void StaticMesh::bindShadowVolume() {
    if (!mCacheData)
      return;

    if (mCacheData->mGlNameVertexArrayObjectShadow)
      glstate::bindVertexArrayObject(mCacheData->mGlNameVertexArrayObjectShadow);
  }

  void StaticMesh::releaseShadowVolume() {
    if (mCacheData && mCacheData->mGlNameVertexArrayObjectShadow)
      glstate::releaseVertexArrayObject(mCacheData->mGlNameVertexArrayObjectShadow);
  }

  void StaticMesh::render(unsigned int drawingMode) {
    if (!mCacheData || mCacheData->mNumUsers < 1)
      return;

    bind();

    glDrawElements(drawingMode, mCacheData->mIndexCount, GL_UNSIGNED_INT, NULL);
    if (config::requiresFlushAfterDraw())
      glFlush();
  }

  size_t StaticMesh::sortingId() const {
    if (!mCacheData)
      return 0;

    return static_cast<size_t>(mCacheData->id());
  }

  primitive::Aabb StaticMesh::recomputeAabb(const glm::vec3 &scale) {
    if (!mCacheData)
      return primitive::Aabb();

    const primitive::Aabb aabb(mCacheData->mAabb);
    const glm::vec3 center = scale * 0.5f * (aabb.mBounds[0] + aabb.mBounds[1]);
    const glm::vec3 extents = scale * 0.5f * (aabb.mBounds[1] - aabb.mBounds[0]);

    return primitive::Aabb(center - extents, center + extents);
  }

  primitive::Sphere StaticMesh::recomputeBoundingSphere(const glm::vec3 &scale) {
    if (!mCacheData)
      return primitive::Sphere();

    const glm::vec3 center = mCacheData->mBoundingSphere.mCenter * scale;
    const float radius = mCacheData->mBoundingSphere.mRadius * std::max(std::max(scale.x, scale.y), scale.z);

    return primitive::Sphere(center, radius);
  }

  void StaticMesh::computeBoundingVolumes() {
    if (!mCacheData)
      return;

    mCacheData->mBoundingSphere = primitive::computeBoundingSphereFromVertices(coords());
    mCacheData->mAabb = primitive::Aabb(coords());
  }

  StaticMesh::StaticMesh() : mIsCachePersistent(false), mCacheData(NULL) {
  }

  static void createOrCompleteEdge(size_t triangleIndex, unsigned int vertexIndex0, unsigned int vertexIndex1,
                                   std::unordered_map<std::pair<size_t, size_t>, Mesh::Edge> &edgeMap,
                                   cache::MeshData *cacheData) {
    // Either add the edge to the edge map if it isn't present, or complete
    // an existing edge if it is already present in the edge map.
    auto itEdgeInverse = edgeMap.find(std::make_pair(vertexIndex1, vertexIndex0));
    if (itEdgeInverse == edgeMap.end()) {
      // If the same edge already exists in the map, add it to the edge list as a non-shared edge
      // before adding this edge to the edge map.
      auto itEdge = edgeMap.find(std::make_pair(vertexIndex0, vertexIndex1));
      if (itEdge != edgeMap.end()) {
        cacheData->mEdges.push_back(itEdge->second);
        edgeMap.erase(itEdge);
      }

      edgeMap.emplace(std::make_pair(vertexIndex0, vertexIndex1), Mesh::Edge(triangleIndex, ~0, vertexIndex0, vertexIndex1));
    } else {
      // If the triangle sharing the edge is co-planar, the edge will never be part of the silhouette
      // so it isn't added to the edge list.
      if (cacheData->mTriangles[itEdgeInverse->second.mTriangleIndices[0]].mNormal !=
          cacheData->mTriangles[triangleIndex].mNormal) {
        itEdgeInverse->second.mTriangleIndices[1] = triangleIndex;
        cacheData->mEdges.push_back(itEdgeInverse->second);
      }
      edgeMap.erase(itEdgeInverse);
    }
  }

  void StaticMesh::computeTrianglesAndEdges() {
    if (!mCacheData)
      return;

    // only triangle-list meshes are allowed to cast shadows
    if (mIndices.size() % 3 != 0)
      return;

    // Merge indices
    std::unordered_map<glm::vec3, unsigned int> coordIndexMap;
    for (size_t i = 0; i < mIndices.size(); ++i) {
      const glm::vec3 coord = mCoords[mIndices[i]];
      auto itCoordIndex = coordIndexMap.find(coord);
      if (itCoordIndex == coordIndexMap.end())
        coordIndexMap[coord] = mIndices[i];
      else
        mIndices[i] = itCoordIndex->second;
    }

    const size_t triangleCount = mIndices.size() / 3;
    mCacheData->mTriangles.reserve(triangleCount);
    mCacheData->mEdges.reserve(triangleCount);
    std::unordered_map<std::pair<size_t, size_t>, Mesh::Edge> edgeMap;
    for (size_t startingIndex = 0, triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex) {
      const size_t index0 = 2 * mIndices[startingIndex++];
      const size_t index1 = 2 * mIndices[startingIndex++];
      const size_t index2 = 2 * mIndices[startingIndex++];
      const glm::vec3 &v0 = mCacheData->mShadowCoords[index0];
      const glm::vec3 &v1 = mCacheData->mShadowCoords[index1];
      const glm::vec3 &v2 = mCacheData->mShadowCoords[index2];

      // eliminate degenerate triangles
      if (v0 == v1 || v1 == v2 || v2 == v0)
        continue;

      // insert triangle into list (compute normal)
      size_t currentTriangleIndex = mCacheData->mTriangles.size();
      glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
      mCacheData->mTriangles.push_back(Mesh::Triangle(index0, index1, index2, normal));

      createOrCompleteEdge(currentTriangleIndex, index0, index1, edgeMap, mCacheData);
      createOrCompleteEdge(currentTriangleIndex, index1, index2, edgeMap, mCacheData);
      createOrCompleteEdge(currentTriangleIndex, index2, index0, edgeMap, mCacheData);
    }

    // DEBUG("Total triangles: " << mCacheData->mTriangles.size());
    // DEBUG("Total shared edges: " << mCacheData->mEdges.size());
    // DEBUG("Edges belonging to a single triangle (non-closed mesh): " << edgeMap.size());

    for (const auto &it : edgeMap)
      mCacheData->mEdges.push_back(it.second);
  }

  void StaticMesh::prepareGl() {
    if (!mCacheData)
      return;

    assert(mCoords.size());
    assert(mIndices.size());

    glGenVertexArrays(1, &mCacheData->mGlNameVertexArrayObject);
    glGenBuffers(1, &mCacheData->mGlNameBufferIndices);

    bind();

    // coordinates
    glGenBuffers(1, &mCacheData->mGlNameBufferCoords);
    glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferCoords);
    glBufferData(GL_ARRAY_BUFFER, mCoords.size() * sizeof(glm::vec3), &mCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(GlslLayout::gLocationCoords, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
    glEnableVertexAttribArray(GlslLayout::gLocationCoords);

    // indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    // normals (optional)
    if (mNormals.size()) {
      assert(mNormals.size() == mCoords.size());

      glGenBuffers(1, &mCacheData->mGlNameBufferNormals);
      glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferNormals);
      glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);
      glVertexAttribPointer(GlslLayout::gLocationNormals, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationNormals);
    }

    // texCoords (optional)
    if (mTexCoords.size()) {
      assert(mTexCoords.size() == mCoords.size());

      glGenBuffers(1, &mCacheData->mGlNameBufferTexCoords);
      glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferTexCoords);
      glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(glm::vec2), &mTexCoords[0], GL_STATIC_DRAW);
      glVertexAttribPointer(GlslLayout::gLocationTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationTexCoords);
    }

    // color per vertex (optional)
    if (mColors.size()) {
      assert(mColors.size() == mCoords.size());

      glGenBuffers(1, &mCacheData->mGlNameBufferColors);
      glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferColors);
      glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(glm::vec3), &mColors[0], GL_STATIC_DRAW);
      glVertexAttribPointer(GlslLayout::gLocationColors, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationColors);
    }

    // Non recursive texCoords (optional)
    if (mUnwrappedTexCoords.size()) {
      assert(mUnwrappedTexCoords.size() == mCoords.size());

      glGenBuffers(1, &mCacheData->mGlNameBufferUnwrappedTexCoords);
      glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferUnwrappedTexCoords);
      glBufferData(GL_ARRAY_BUFFER, mUnwrappedTexCoords.size() * sizeof(glm::vec2), &mUnwrappedTexCoords[0], GL_STATIC_DRAW);
      glVertexAttribPointer(GlslLayout::gLocationUnwrappedTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationUnwrappedTexCoords);
    }

    // shadow volume buffers (coords and indices)
    glGenVertexArrays(1, &mCacheData->mGlNameVertexArrayObjectShadow);

    bindShadowVolume();

    // even indices store original vertex coord, odd indices store vertex coord to be extruded in vertex shader
    mCacheData->mShadowCoords.resize(mCoords.size() * 2);
    for (size_t i = 0; i < mCoords.size(); ++i) {
      mCacheData->mShadowCoords[2 * i] = glm::vec4(mCoords[i], 1.0f);
      mCacheData->mShadowCoords[2 * i + 1] = glm::vec4(mCoords[i], 0.0f);
    }

    glGenBuffers(1, &mCacheData->mGlNameBufferShadowCoords);
    glBindBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferShadowCoords);
    glBufferData(GL_ARRAY_BUFFER, mCacheData->mShadowCoords.size() * sizeof(glm::vec4), &mCacheData->mShadowCoords[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(GlslLayout::gLocationCoords, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL);
    glEnableVertexAttribArray(GlslLayout::gLocationCoords);

    if (mCoords.size() <= config::maxVerticesPerMeshForShadowRendering() && config::areShadowsEnabled()) {
      computeTrianglesAndEdges();
      mCacheData->mSupportShadows = true;
    } else
      mCacheData->mSupportShadows = false;

    // only the number of indices needs to be known in order to render the mesh
    // vertex count kept for mesh export
    mCacheData->mIndexCount = mIndices.size();
    mCacheData->mVertexCount = mCoords.size();

    mCoords.clear();
    mIndices.clear();
    mNormals.clear();
    mTexCoords.clear();
    mColors.clear();
  }

  void StaticMesh::cleanupGl() {
    if (!mCacheData)
      return;

    --mCacheData->mNumUsers;

    if (!mCacheData->mNumUsers) {
      if (mCacheData->mGlNameVertexArrayObject) {
        release();
        releaseShadowVolume();

        glDeleteVertexArrays(1, &mCacheData->mGlNameVertexArrayObject);
        mCacheData->mGlNameVertexArrayObject = 0;
        if (mCacheData->mGlNameVertexArrayObjectShadow) {
          glDeleteVertexArrays(1, &mCacheData->mGlNameVertexArrayObjectShadow);
          mCacheData->mGlNameVertexArrayObjectShadow = 0;
        }
        if (mCacheData->mGlNameBufferCoords) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferCoords);
          mCacheData->mGlNameBufferCoords = 0;
        }
        if (mCacheData->mGlNameBufferIndices) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferIndices);
          mCacheData->mGlNameBufferIndices = 0;
        }
        if (mCacheData->mGlNameBufferShadowCoords) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferShadowCoords);
          mCacheData->mGlNameBufferShadowCoords = 0;
        }

        if (mCacheData->mGlNameBufferNormals) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferNormals);
          mCacheData->mGlNameBufferNormals = 0;
        }

        if (mCacheData->mGlNameBufferTexCoords) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferTexCoords);
          mCacheData->mGlNameBufferTexCoords = 0;
        }

        if (mCacheData->mGlNameBufferColors) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferColors);
          mCacheData->mGlNameBufferColors = 0;
        }

        if (mCacheData->mGlNameBufferUnwrappedTexCoords) {
          glDeleteBuffers(1, &mCacheData->mGlNameBufferUnwrappedTexCoords);
          mCacheData->mGlNameBufferUnwrappedTexCoords = 0;
        }
      }

      StaticMesh::cCache.erase(mCacheKey);
    }

    mCacheKey = cache::Key();
    mCacheData = NULL;
  }

  static void copyFromBuffer(unsigned int target, unsigned int buffer, size_t size, void *dest) {
    glBindBuffer(target, buffer);
#ifndef __EMSCRIPTEN__
    glGetBufferSubData(target, 0, size, dest);
#endif
  }

  /*
    This function returns a copy of the mesh data, either from the CPU or the GPU depending on whether the CPU -> GPU
    transfer has been made or not (mesh data is cleared after being uploaded to the GPU).
    As this function makes an intensive use of memory transfer, it should not be used in performance sensitive code.
  */
  void StaticMesh::readData(float *coordData, float *normalData, float *texCoordData, unsigned int *indexData) {
    // Check whether prepareGl has been called
    // (it is very unlikely that the condition will be met, but it has to be checked).
    if (mCoords.size() > 0) {
      const int vertexCounter = mCoords.size();
      const int indexCounter = mIndices.size();

      if (coordData)
        memcpy(coordData, &mCoords[0], vertexCounter * sizeof(glm::vec3));
      if (normalData)
        memcpy(normalData, &mNormals[0], vertexCounter * sizeof(glm::vec3));
      if (texCoordData)
        memcpy(texCoordData, &mTexCoords[0], vertexCounter * sizeof(glm::vec2));
      if (indexData)
        memcpy(indexData, &mIndices[0], indexCounter * sizeof(unsigned int));
    } else {
      const int vertexCounter = mCacheData ? mCacheData->mVertexCount : 0;
      const int indexCounter = mCacheData ? mCacheData->mIndexCount : 0;

      bind();

      if (indexData && mCacheData)
        copyFromBuffer(GL_ELEMENT_ARRAY_BUFFER, mCacheData->mGlNameBufferIndices, indexCounter * sizeof(unsigned int),
                       indexData);
      if (coordData && mCacheData)
        copyFromBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferCoords, vertexCounter * sizeof(glm::vec3), coordData);
      if (normalData && mCacheData)
        copyFromBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferNormals, vertexCounter * sizeof(glm::vec3), normalData);
      if (texCoordData && mCacheData)
        copyFromBuffer(GL_ARRAY_BUFFER, mCacheData->mGlNameBufferTexCoords, vertexCounter * sizeof(glm::vec2), texCoordData);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      release();
    }
  }

  int StaticMesh::vertexCount() const {
    if (mCoords.size() > 0)
      return mCoords.size();
    else if (mCacheData)
      return mCacheData->mVertexCount;
    return 0;
  }

  int StaticMesh::indexCount() const {
    if (mIndices.size() > 0)
      return mIndices.size();
    else if (mCacheData)
      return mCacheData->mIndexCount;
    return 0;
  }

}  // namespace wren

// C interface implementation
WrStaticMesh *wr_static_mesh_unit_box_new(bool outline) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createUnitBox(outline));
}

WrStaticMesh *wr_static_mesh_unit_cone_new(int subdivision, bool has_side, bool has_bottom) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createUnitCone(subdivision, has_side, has_bottom));
}

WrStaticMesh *wr_static_mesh_unit_cylinder_new(int subdivision, bool has_side, bool has_top, bool has_bottom, bool outline) {
  return reinterpret_cast<WrStaticMesh *>(
    wren::StaticMesh::createUnitCylinder(subdivision, has_side, has_top, has_bottom, outline));
}

WrStaticMesh *wr_static_mesh_unit_elevation_grid_new(int dimension_x, int dimension_y, const float *height_data,
                                                     float thickness, bool outline) {
  return reinterpret_cast<WrStaticMesh *>(
    wren::StaticMesh::createUnitElevationGrid(dimension_x, dimension_y, height_data, thickness, outline));
}

WrStaticMesh *wr_static_mesh_unit_rectangle_new(bool outline) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createUnitRectangle(outline));
}

WrStaticMesh *wr_static_mesh_quad_new() {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createQuad());
}

WrStaticMesh *wr_static_mesh_unit_sphere_new(int subdivision, bool ico, bool outline) {
  if (ico)
    return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createUnitIcosphere(subdivision, outline));
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createUnitUVSphere(subdivision, outline));
}

WrStaticMesh *wr_static_mesh_capsule_new(int subdivision, float radius, float height, bool has_side, bool has_top,
                                         bool has_bottom, bool outline) {
  return reinterpret_cast<WrStaticMesh *>(
    wren::StaticMesh::createCapsule(subdivision, radius, height, has_side, has_top, has_bottom, outline));
}

WrStaticMesh *wr_static_mesh_line_set_new(int coord_count, const float *coord_data, const float *color_data) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createLineSet(coord_count, coord_data, color_data));
}

WrStaticMesh *wr_static_mesh_point_pre_set_new(int coord_count, const float *coord_data, const float *color_data, float *&coordPtr, float *&colorPtr) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::preCreatePointSet(coord_count, coord_data, color_data, coordPtr, colorPtr));
}

void wr_static_mesh_point_post_set_new(WrStaticMesh *mesh) {
  wren::StaticMesh::postCreatePointSet(reinterpret_cast<wren::StaticMesh*>(mesh));
}

WrStaticMesh *wr_static_mesh_point_set_new(int coord_count, const float *coord_data, const float *color_data) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createPointSet(coord_count, coord_data, color_data));
}

WrStaticMesh *wr_static_mesh_new(int vertex_count, int index_count, const float *coord_data, const float *normal_data,
                                 const float *tex_coord_data, const float *unwrapped_tex_coord_data,
                                 const unsigned int *index_data, bool outline) {
  return reinterpret_cast<WrStaticMesh *>(wren::StaticMesh::createTriangleMesh(
    vertex_count, index_count, coord_data, normal_data, tex_coord_data, unwrapped_tex_coord_data, index_data, outline));
}

void wr_static_mesh_delete(WrStaticMesh *mesh) {
  wren::StaticMesh::deleteMesh(reinterpret_cast<wren::Mesh *>(mesh));
}

void wr_static_mesh_get_bounding_sphere(WrStaticMesh *mesh, float *sphere) {
  const wren::primitive::Sphere s = reinterpret_cast<wren::Mesh *>(mesh)->recomputeBoundingSphere();
  sphere[0] = s.mCenter.x;
  sphere[1] = s.mCenter.y;
  sphere[2] = s.mCenter.z;
  sphere[3] = s.mRadius;
}

void wr_static_mesh_read_data(WrStaticMesh *mesh, float *coord_data, float *normal_data, float *tex_coord_data,
                              unsigned int *index_data) {
  reinterpret_cast<wren::StaticMesh *>(mesh)->readData(coord_data, normal_data, tex_coord_data, index_data);
}

int wr_static_mesh_get_vertex_count(WrStaticMesh *mesh) {
  return reinterpret_cast<wren::StaticMesh *>(mesh)->vertexCount();
}

int wr_static_mesh_get_index_count(WrStaticMesh *mesh) {
  return reinterpret_cast<wren::StaticMesh *>(mesh)->indexCount();
}
