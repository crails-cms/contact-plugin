#pragma once
#include "app/controllers/admin/application.hpp"
#include "app/models/contact_page.hpp"
#include "app/models/user.hpp"
#include <crails/cms/controllers/admin.hpp>

typedef Crails::Cms::AdminController<User, AdminApplicationController> AdminContactPageControllerBase;

class AdminContactPageController : public AdminContactPageControllerBase
{
  typedef AdminContactPageControllerBase Super;
public:
  static constexpr const char* scope = "contact_page";

  AdminContactPageController(Crails::Context& context);

  std::string get_view_scope() const { return scope; }

  void initialize() override;

  void index() { show(); }
  void show();
  void new_() { show(); }
  void create() { update(); }
  void update();
  void destroy();

private:
  std::shared_ptr<ContactPage> contact_page;
};
