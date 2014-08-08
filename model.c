#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include "maths.h"

#define AI_CONFIG_PP_RVC_FLAGS aiComponent_NORMALS
#define AI_CONFIG_PP_SBP_REMOVE aiPrimitiveType_LINE | aiPrimitiveType_POINT

const struct aiScene *loadModel(const char *file)
{
  const struct aiScene *scene = aiImportFile(file,  aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_GenNormals);
  return scene;
}


const struct aiScene *loadTextQuad(const char *file)
{
  const struct aiScene *scene = aiImportFile(file,  aiProcess_RemoveComponent);
  return scene;
}


void drawModel(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha)
{
  GLint i;
  struct aiMesh *mesh;

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
  glScalef(size, size, size);
  glColor4ub(125, 125, 125, alpha);
  for (i = 0; i < scene->mNumMeshes; i++) {
    mesh = scene->mMeshes[i];
    glVertexPointer(3, GL_FLOAT, 0, mesh->mVertices);
    glNormalPointer(GL_FLOAT, 0, mesh->mNormals);
    glTexCoordPointer(3, GL_FLOAT, 0, mesh->mTextureCoords[0]);
    //glMultiTexCoordPointerEXT(1, 3, GL_FLOAT, 0, mesh->mTextureCoords[0]);
    glDrawArrays(GL_TRIANGLES, 0, mesh->mNumFaces * 3/*mesh->mFaces[0].mNumIndices*/);
  }
  glPopMatrix();
}
