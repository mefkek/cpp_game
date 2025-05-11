#pragma once
#include "Logger.hpp"

class ManagerNotFoundException : public std::exception
{
    private:
    std::string msg;
    public:
    ManagerNotFoundException(std::string type_name);
    const char* what() const noexcept override;
};

class FileNotFoundException : public std::exception
{
    private:
    std::string filepath;
    public:
    FileNotFoundException(std::string path);
    const char* what() const noexcept override;
};

class DuplicateRenderLayerException : public std::exception
{
    /*
        if (two layers with the same name) then
            dont
        endif
    */
    private:
    std::string message;
    public:
    DuplicateRenderLayerException(const std::string& name);
    const char* what() const noexcept override;
};

class NoWindowGivenException: public std::exception
{
    /*
        for when there is no window given
    */
    public:
    const char* what() const noexcept override;
};