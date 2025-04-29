#include "Utility/Exceptions.hpp"

ManagerNotFoundException::ManagerNotFoundException(std::string type_name)
{
    msg = "Manager of type: " + type_name + " has not been found.";
}

const char* ManagerNotFoundException::what() const noexcept
{
    return msg.c_str();
}

FileNotFoundException::FileNotFoundException(std::string path) : filepath(path) {}

const char* FileNotFoundException::what() const noexcept
{
    std::string msg("Font at path \"" + filepath + "\" has not been found.");
    return msg.c_str(); 
}

DuplicateRenderLayerException::DuplicateRenderLayerException(const std::string& name)
{
    message = "Attempted to create a render layer with duplicate name: " +  name + ".";
}

const char* DuplicateRenderLayerException::what() const noexcept
{
    return message.c_str();
}

const char* NoWindowGivenException::what() const noexcept
{
    std::string msg("Render manger: no window given.");
    return msg.c_str();
}