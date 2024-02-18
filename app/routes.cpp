#include <crails/router.hpp>
#include <crails/cms/routes.hpp>
#include <crails/cms/views/admin_menu.hpp>
#include "controllers/contact.hpp"
#include "controllers/admin/contact_page.hpp"
#include "../lib/plugin-odb.hxx"

void initialize_plugin_routes()
{
  auto& router = Crails::Router::singleton::require();
  auto& cms_routes = Crails::Cms::Routes::singleton::require();
  auto& admin_menu = Crails::Cms::AdminMenu::singleton::require();

  router.scope("/contact", [&]()
  {
    cms_routes.set_path_for<ContactController>(router);
    router.match_action("GET", "/", ContactController, show);
    router.match_action("POST", "/", ContactController, submit);
  });

  router.scope(AdminApplicationController::scope, [&]()
  {
    router.scope("/contact", [&]()
    {
      cms_routes.set_path_for<AdminContactPageController>(router);
      admin_menu.add({4, "contact_page", "/admin/contact"});
      router.libcrails_cms_admin_crud("/", AdminContactPageController);
    });
  });
}
