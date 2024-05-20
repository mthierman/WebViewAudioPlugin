#pragma once

#include <gui/choc_WebView.h>
#include <network/choc_MIMETypes.h>
#include "BinaryData.h"

struct Resource
{
    Resource() = default;
    Resource(std::string path, std::string resourceName);

    std::string m_path;
    choc::ui::WebView::Options::Resource m_resource;
};

Resource::Resource(std::string path, std::string resourceName) : m_path{path}
{
    int dataSize{};
    auto namedResource{BinaryData::getNamedResource(resourceName.c_str(), dataSize)};

    m_resource = choc::ui::WebView::Options::Resource{
        std::string(namedResource, dataSize),
        choc::network::getMIMETypeFromFilename(
            BinaryData::getNamedResourceOriginalFilename(resourceName.c_str()))};
}
