#include <crails/renderer.hpp>
#include "../lib/renderers/contact_plugin_html_renderer.hpp"
#include <iostream>

static void initialize_renderer(const Crails::Renderers& renderers, const Crails::Renderer& source)
{
  for (auto it = renderers.begin() ; it != renderers.end() ; ++it)
  {
    const Crails::Renderer* renderer = it->get();

    if (*renderer->get_mimetypes().begin() == *source.get_mimetypes().begin())
      const_cast<Crails::Renderer*>(renderer)->merge(source);
  }
}

void initialize_plugin_renderers()
{
  auto& renderers = Crails::Renderers::singleton::require();
  ContactPluginHtmlRenderer plugin_html;

  initialize_renderer(renderers, plugin_html);
}
