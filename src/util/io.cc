#include <fstream>
#include <sstream>

#include "util.hh"


// Out: Print to stdout.

void
Util::Out(const std::string &s)
{
  std::cout << s << std::endl;
}


// Warn: Print warning to stderr.

void
Util::Warn(const std::string &s)
{
  std::cerr << "Warning: " << s << std::endl;
}


// Err: Print error to stderr.

void
Util::Err(const std::string &s)
{
  std::cerr << "Error: " << s << std::endl;
}


// ErrGl: Print gl error to stderr.

void
Util::ErrGl(const std::string &s)
{
  std::cerr << "Error (gl): " << s << std::endl;
}


/**
// LoadShader: Parse a file containing shaders.

LoadShaderOut
Util::LoadShader(const std::string &path)
{
  enum class Type { NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2 };
  std::stringstream ss[3];
  std::ifstream stream(path);
  if (stream)
  {
    Type type = Type::NONE;
    std::string line;
    while(std::getline(stream, line))
    {
      if ("# vertex" == line)
      {
        type = Type::VERTEX;
      }
      else if ("# geometry" == line)
      {
        type = Type::GEOMETRY;
      }
      else if ("# fragment" == line)
      {
        type = Type::FRAGMENT;
      }
      else if (line.rfind("//", 0) != 0)
      {
        ss[static_cast<int>(type)] << line << '\n';
      }
    }
    stream.close();
  }
  else
  {
    return { "", "" };
  }
  return { Util::Trim(ss[static_cast<int>(Type::VERTEX)].str()),
           Util::Trim(ss[static_cast<int>(Type::GEOMETRY)].str()),
           Util::Trim(ss[static_cast<int>(Type::FRAGMENT)].str()) };
}
//*/


// LoadState: Parse a file containing the initial state.

bool
Util::LoadState(State &state, const std::string &path)
{
  std::ifstream stream(path);
  if (stream)
  {
    std::string line;
    std::istringstream linestream;
    std::getline(stream, line);
    if (! line.empty())
    {
      unsigned int stop;
      unsigned int width;
      unsigned int height;
      int distribution_num;
      Distribution distribution;
      float scope;
      float speed;
      float alpha;
      float beta;
      linestream = std::istringstream(line);
      // on read failure, the State members are left unchanged
      if (linestream >> stop) state.stop_ = stop;
      if (linestream >> width) state.width_ = width;
      if (linestream >> height) state.height_ = height;
      if (linestream >> distribution_num)
        state.distribution_ = static_cast<Distribution>(distribution_num);
      if (linestream >> scope) state.scope_ = scope;
      if (linestream >> speed) state.speed_ = speed;
      if (linestream >> alpha) state.alpha_ = alpha;
      if (linestream >> beta) state.beta_ = beta;
    }
    unsigned int x;
    unsigned int y;
    float phi;
    unsigned size;
    state.particles_.clear();
    while (std::getline(stream, line))
    {
      if (line.empty())
      {
        continue;
      }
      linestream = std::istringstream(line);
      Particle particle(state.distribution_, state.width_,
                        state.height_);
      if (linestream >> x) particle.x = x;
      if (linestream >> y) particle.y = y;
      if (linestream >> phi) particle.phi = phi;
      if (linestream >> size) particle.size = size;
      state.particles_.push_back(particle);
    }
    if (state.particles_.empty())
    {
      state.particles_ = std::vector<Particle>
        (1000, Particle(state.distribution_, state.width_,
                        state.height_));
    }
  }
  else
  {
    return false;
  }
  return true;
}


// SaveState: Write the current state to a file.

bool
Util::SaveState(State &state, const std::string &path)
{
  std::ofstream stream(path);
  if (stream)
  {
    stream << state.stop_ << ' '
           << state.width_ << ' '
           << state.height_ << ' '
           << static_cast<int>(state.distribution_) << ' '
           << state.scope_ << ' '
           << state.speed_ << ' '
           << state.alpha_ << ' '
           << state.beta_ << '\n';
    for (const Particle &particle : state.particles_)
    {
      stream << particle.x << ' '
             << particle.y << ' '
             << particle.phi << ' '
             << particle.size << '\n';
    }
    stream.close();
  }
  else
  {
    return false;
  }
  return true;
}

