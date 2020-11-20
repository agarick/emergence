#include <GL/glew.h>

#include "gl.hh"
#include "../util/util.hh"


// VertexBuffer

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
  : data_(data), size_(size)
{
  DOGL(glGenBuffers(1, &this->id_));
}

VertexBuffer::~VertexBuffer()
{
  DOGL(glDeleteBuffers(1, &this->id_));
}

void
VertexBuffer::Bind() const
{
  DOGL(glBindBuffer(GL_ARRAY_BUFFER, this->id_));
}

void
VertexBuffer::Buffer() const
{
  DOGL(glBufferData(GL_ARRAY_BUFFER, this->size_, this->data_, GL_STATIC_DRAW));
}

void
VertexBuffer::Update(const void* data)
{
  this->data_ = data;
  //this->size_ = sizeof(data);
  this->Bind();
  this->Buffer();
}

void
VertexBuffer::Unbind() const
{
  DOGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


// IndexBuffer

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
  : count_(count)
{
  DOGL(glGenBuffers(1, &this->id_));
  DOGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id_));
  DOGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                    data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  DOGL(glDeleteBuffers(1, &this->id_));
}

void
IndexBuffer::Bind() const
{
  DOGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id_));
}

void
IndexBuffer::Unbind() const
{
  DOGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


// LayoutItem

// TypeSize: Convenience function that converts OpenGL data
//           constants to corresponding data sizes.

unsigned int
LayoutItem::TypeSize(unsigned int type)
{
  switch (type)
  {
    case GL_FLOAT:         return sizeof(GL_FLOAT);
    case GL_UNSIGNED_INT:  return sizeof(GL_UNSIGNED_INT);
    case GL_UNSIGNED_BYTE: return sizeof(GL_UNSIGNED_BYTE);
  }
  return 0;
}


// VertexBufferLayout

template<typename T> LayoutItem
VertexBufferLayout::Make(unsigned int count)
{
  //static_assert(false);
}

template<> LayoutItem
VertexBufferLayout::Make<float>(unsigned int count)
{
  return { GL_FLOAT, count, GL_FALSE,
           count * LayoutItem::TypeSize(GL_FLOAT) };
}

template<> LayoutItem
VertexBufferLayout::Make<unsigned int>(unsigned int count)
{
  return { GL_UNSIGNED_INT, count, GL_FALSE,
           count * LayoutItem::TypeSize(GL_UNSIGNED_INT) };
}

template<> LayoutItem
VertexBufferLayout::Make<unsigned char>(unsigned int count)
{
  return { GL_UNSIGNED_BYTE, count, GL_TRUE,
           count * LayoutItem::TypeSize(GL_UNSIGNED_BYTE) };
}


// VertexArray

VertexArray::VertexArray()
{
  DOGL(glGenVertexArrays(1, &this->id_));
}

VertexArray::~VertexArray()
{
  DOGL(glDeleteVertexArrays(1, &this->id_));
}

void
VertexArray::AddBuffer(unsigned int id, const VertexBuffer &vb,
                       const LayoutItem &item)
{
  this->Bind();
  vb.Bind();
  vb.Buffer();
  DOGL(glEnableVertexAttribArray(id));
  DOGL(glVertexAttribPointer(id, item.count, item.type, item.normalised,
                             item.stride, static_cast<const void*>(0)));
}

void
VertexArray::Bind() const
{
  DOGL(glBindVertexArray(this->id_));
}

void
VertexArray::Unbind() const
{
  DOGL(glBindVertexArray(0));
}


// UniformBuffer

UniformBuffer::UniformBuffer()
  : id_(0)
{
}


// Shader

Shader::Shader()
  : id_(0)
{
  this->id_ = Shader::CreateShader();
}

Shader::~Shader()
{
  DOGL(glDeleteProgram(this->id_));
}

void
Shader::Bind() const
{
  DOGL(glUseProgram(this->id_));
}

void
Shader::Unbind() const
{
  DOGL(glUseProgram(0));
}

unsigned int
Shader::CreateShader()
{
  std::string vertex =
    "#version 330 core\n"
    "layout (location = 0) in vec3 xyz;\n"
    "layout (location = 1) in vec2 quad;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "  vec4 position = vec4(xyz.x + quad.x, xyz.y + quad.y, xyz.z, 1.0f);\n"
    "  gl_Position = mvp * position;\n"
    "}\n";

  std::string geometry = "";

  std::string fragment =
    "#version 330 core\n"
    "layout (location = 0) out vec4 color;\n"
    "void main()\n"
    "{\n"
    "  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n";

  /**
  std::string vertex =
    "#version 330 core\n"
    "layout (location = 0) in vec3 xyz;\n"
    "layout (location = 1) in vec3 rgb;\n"
    "out vec3 frag_color;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = mvp * vec4(xyz, 1.0f);\n"
    "  frag_color = rgb;\n"
    "}\n";
  std::string geometry = "";
  std::string fragment =
    "#version 330 core\n"
    "in vec3 frag_color;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "  color = vec4(frag_color, 1.0f);\n"
    "}\n";
  //*/

  DOGL(unsigned int program = glCreateProgram());
  unsigned int v;
  unsigned int g;
  unsigned int f;
  if (! vertex.empty())
  {
    v = Shader::CompileShader(GL_VERTEX_SHADER, vertex);
    DOGL(glAttachShader(program, v));
  }
  if (! geometry.empty())
  {
    g = Shader::CompileShader(GL_GEOMETRY_SHADER, geometry);
    DOGL(glAttachShader(program, g));
  }
  if (! fragment.empty())
  {
    f = Shader::CompileShader(GL_FRAGMENT_SHADER, fragment);
    DOGL(glAttachShader(program, f));
  }
  DOGL(glLinkProgram(program));
  DOGL(glValidateProgram(program));
  if (! vertex.empty()) { DOGL(glDeleteShader(v)); }
  if (! geometry.empty()) { DOGL(glDeleteShader(g)); }
  if (! fragment.empty()) { DOGL(glDeleteShader(f)); }
  return program;
}

unsigned int
Shader::CompileShader(unsigned int type, const std::string &source)
{
  DOGL(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  DOGL(glShaderSource(id, 1, &src, nullptr));
  DOGL(glCompileShader(id));
  int success;
  DOGL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
  if (GL_FALSE == success)
  {
    int length;
    DOGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = static_cast<char*>(alloca(length * sizeof(char)));
    DOGL(glGetShaderInfoLog(id, length, &length, message));
    Util::Err("failed to compile "
              + std::string(type == GL_VERTEX_SHADER ? "vertex" :
                            type == GL_GEOMETRY_SHADER ? "geometry" :
                            "fragment")
              + '\n' + message);
    DOGL(glDeleteShader(id));
    return 0;
  }
  return id;
}

int
Shader::GetUniformLocation(const std::string &name)
{
  if (this->uniform_location_cache_.find(name) !=
      this->uniform_location_cache_.end())
  {
    return this->uniform_location_cache_[name];
  }
  DOGL(int location = glGetUniformLocation(this->id_, name.c_str()));
  if (-1 == location)
  {
    Util::Warn("uniform '" + name + "' not found");
  }
  this->uniform_location_cache_[name] = location;
  return location;
}

void
Shader::SetUniform1f(const std::string &name, float v)
{
  DOGL(glUniform1f(Shader::GetUniformLocation(name), v));
}

void
Shader::SetUniform4f(const std::string &name,
                     float v0, float v1, float v2, float v3)
{
  DOGL(glUniform4f(Shader::GetUniformLocation(name), v0, v1, v2, v3));
}

void
Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &mat)
{
  DOGL(glUniformMatrix4fv(Shader::GetUniformLocation(name), 1, GL_FALSE,
                          &mat[0][0]));
}

