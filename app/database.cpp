#include <crails/odb/connection.hpp>

void install_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.execute(
    "CREATE TABLE \"ContactPage\" ("
      "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
      "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
      "\"created_at\" BIGINT NULL,"
      "\"updated_at\" BIGINT NULL,"
      "\"sender\" TEXT NOT NULL,"
      "\"address\" TEXT NOT NULL,"
      "\"content\" TEXT NOT NULL"
    ");"
  );
  database.commit();
}

void uninstall_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.execute("DROP TABLE IF EXISTS \"ContactPage\" CASCADE;");
  database.commit();
}
