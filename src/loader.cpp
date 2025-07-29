//
// Created by AmazingBuff on 25-7-26.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

RASTERIZER_NAME_SPACE_BEGIN

Vector<Mesh> load_mesh(String const& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    Vector<Mesh> ret;
    if (scene)
    {
        ret.resize(scene->mNumMeshes);
        for (uint32_t i = 0; i < scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            ret[i].vertices.resize(mesh->mNumVertices);

            if (mesh->HasPositions())
            {
                for (uint32_t j = 0; j < mesh->mNumVertices; j++)
                {
                    ret[i].vertices[j].position.x() = mesh->mVertices[j].x;
                    ret[i].vertices[j].position.y() = mesh->mVertices[j].y;
                    ret[i].vertices[j].position.z() = mesh->mVertices[j].z;
                }
            }

            if (mesh->HasTextureCoords(0))
            {
                for (uint32_t j = 0; j < mesh->mNumVertices; j++)
                {
                    ret[i].vertices[j].tex_coord.x() = mesh->mTextureCoords[0][j].x;
                    ret[i].vertices[j].tex_coord.y() = mesh->mTextureCoords[0][j].y;
                }
            }

            if (mesh->HasNormals())
            {
                for (uint32_t j = 0; j < mesh->mNumVertices; j++)
                {
                    ret[i].vertices[j].normal.x() = mesh->mNormals[j].x;
                    ret[i].vertices[j].normal.y() = mesh->mNormals[j].y;
                    ret[i].vertices[j].normal.z() = mesh->mNormals[j].z;
                }
            }

            if (mesh->HasTangentsAndBitangents())
            {
                for (uint32_t j = 0; j < mesh->mNumVertices; j++)
                {
                    ret[i].vertices[j].tangent.x() = mesh->mTangents[j].x;
                    ret[i].vertices[j].tangent.y() = mesh->mTangents[j].y;
                    ret[i].vertices[j].tangent.z() = mesh->mTangents[j].z;
                }
            }

            if (mesh->HasFaces())
            {
                ret[i].indices.resize(mesh->mNumFaces);
                for (uint32_t j = 0; j < mesh->mNumFaces; j++)
                {
                    const aiFace& faces = mesh->mFaces[j];
                    ret[i].indices[j] = {static_cast<int>(faces.mIndices[0]), static_cast<int>(faces.mIndices[1]), static_cast<int>(faces.mIndices[2])};
                }
            }
        }
    }

    return ret;
}

Texture load_image(String const& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    Texture ret{
        .width = static_cast<uint32_t>(width),
        .height = static_cast<uint32_t>(height),
        .mip_levels = 1
    };
    ret.pixels.resize(width * height);
    memcpy(ret.pixels.data(), image, width * height * sizeof(Pixel));
    stbi_image_free(image);
    return ret;
}

RASTERIZER_NAME_SPACE_END