#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include "maths.h"

#define AI_CONFIG_PP_RVC_FLAGS aiComponent_NORMALS
#define AI_CONFIG_PP_SBP_REMOVE aiPrimitiveType_LINE | aiPrimitiveType_POINT

const struct aiScene *loadFromOBJFile(const char *name)
{
  const struct aiScene *foo = aiImportFile(name,  aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_GenNormals);
  return foo;
}


void drawModel(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha)
{
  GLuint k;
  GLint h, i, j;
  struct aiMesh *mesh;
  struct aiFace *face;

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
  glScalef(size, size, size);
  glColor4ub(125, 125, 125, alpha);
  for (h = 0; h < scene->mNumMeshes; h++) {
    mesh = scene->mMeshes[h];
    glVertexPointer(3, GL_FLOAT, 0, mesh->mVertices);
    glNormalPointer(GL_FLOAT, 0, mesh->mNormals);
    glTexCoordPointer(3, GL_FLOAT, 0, mesh->mTextureCoords[0]);
    glDrawArrays(GL_TRIANGLES, 0, mesh->mNumFaces * 3/*mesh->mFaces[0].mNumIndices*/);
    /*glBegin(GL_TRIANGLES);
    for (k = 0; k < mesh->mNumFaces; k++) {
      face = &mesh->mFaces[k];
      for (i = 0; i < face->mNumIndices; i++) {
        j = face->mIndices[i];
        glTexCoord2f(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
        glArrayElement(j);
      }
    }
    glEnd();*/
  }
  glPopMatrix();
}
