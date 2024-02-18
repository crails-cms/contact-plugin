#include <crails/any_cast.hpp>
#include "contact_page.hpp"
#include "lib/contact_page-odb.hxx"

using namespace std;
  
AdminContactPageController::AdminContactPageController(Crails::Context& context) : Super(context)
{
  vars["page_title"] = i18n::t("admin.menu.contact_page");
  vars["page_subtitle"] = i18n::t("admin.contact_page.subtitle");
  vars["local_route"] = std::string(AdminApplicationController::scope) + "/contact";
}

void AdminContactPageController::initialize()
{
  Super::initialize();
  if (!database.find_one(contact_page))
    contact_page = make_shared<ContactPage>();
}

void AdminContactPageController::show()
{
  render("admin/" + get_view_scope() + "/show", {
    {"model", reinterpret_cast<const ContactPage*>(contact_page.get())}
  });
}

void AdminContactPageController::update()
{
  contact_page->edit(params[ContactPage::scope]);
  database.save(*contact_page);
  database.commit();
  flash["info"] = i18n::t("admin.flash.resource-updated");
  redirect_to(Crails::any_cast(vars["local_route"]));
}

void AdminContactPageController::destroy()
{
  database.destroy(*contact_page);
  redirect_to_root();
}
