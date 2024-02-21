#include <crails/i18n_settings.hpp>
#include <string>

void initialize_plugin_routes();
void initialize_plugin_renderers();
void install_plugin_database();
void uninstall_plugin_database();

extern "C"
{
  void initialize()
  {
    i18n::Settings& tsettings = i18n::Settings::singleton::require();

    for (const auto& locale : std::vector<std::string>{"fr", "en", "es"})
      tsettings.t.add_locale(locale, "locales/contact." + locale + ".json");
    initialize_plugin_routes();
    initialize_plugin_renderers();
  }

  void install()
  {
    install_plugin_database();
  }

  void uninstall()
  {
    uninstall_plugin_database();
  }
}
