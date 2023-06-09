#include "Mesh.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

bool Mesh::LoadFromObj(const char* filename)
{
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = 3;

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                Vertex new_vert;
                new_vert.position.x = vx;
                new_vert.position.y = vy;
                new_vert.position.z = vz;

                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

                    new_vert.normal.x = nx;
                    new_vert.normal.y = ny;
                    new_vert.normal.z = nz;
                }

                // if (idx.texcoord_index >= 0)
                //{
                //     tinyobj::real_t ux = attrib.texcoords[2 * idx.texcoord_index + 0];
                //     tinyobj::real_t uy = attrib.texcoords[2 * idx.texcoord_index + 1];

                //    new_vert.uv.x = ux;
                //    new_vert.uv.y = 1 - uy;
                //}

                // new_vert.color = new_vert.normal;
                vertices.push_back(new_vert);
            }
            index_offset += fv;
        }
    }

    SetupMesh();

    ModelMatrix = CalculateModelMatrix();

    return true;
}

Mesh::Mesh(const char* filename)
{
    LoadFromObj(filename);
}

void Mesh::SetColor(glm::vec4 Color)
{
    this->Color = Color;
}

void Mesh::SetPosition(glm::vec3 Position)
{
    this->Position = Position;
    ModelMatrix = CalculateModelMatrix();
}

void Mesh::ScaleMesh(glm::vec3 Scale)
{
    this->Scale = Scale;
    ModelMatrix = CalculateModelMatrix();
}

glm::mat4 Mesh::GetModelMatrix()
{
    return ModelMatrix;
}

void Mesh::Move(glm::vec3 Offset)
{
    Position += Offset;
    ModelMatrix = CalculateModelMatrix();
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind Vertex Array Object and VBO in correct order
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // VBO stuff
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Vertex position pointer init
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normal pointer init
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex texture coord
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Unbinding VAO
    glBindVertexArray(0);
}

glm::mat4 Mesh::CalculateModelMatrix()
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, Position);

    model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, Scale);

    return model;
}
