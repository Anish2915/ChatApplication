#include "Mongo.h"

#include <string>
#include <string_view>
#include <cassert>

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/exception/exception.hpp>


std::string mongo::GetIpAddressOfServer()
{
    try {
        mongocxx::instance inst{};
        const auto uri = mongocxx::uri{  };

        mongocxx::options::client client_options;
        const auto api = mongocxx::options::server_api{ mongocxx::options::server_api::version::k_version_1 };
        client_options.server_api_opts(api);

        mongocxx::client conn{ uri, client_options };
        mongocxx::database db = conn["userdata"];


        const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        db.run_command(ping_cmd.view());
        std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;

        auto collection = db["IP"];

        bsoncxx::builder::stream::document document{};
        auto value = document << "_id" << bsoncxx::oid("65e5f0eb4eedd387f93ee1c9") << bsoncxx::builder::stream::finalize;

        auto tt = collection.find(value.view());

        for (auto&& it : tt) {
            for (bsoncxx::document::element& elem : it) {
                bsoncxx::stdx::string_view field_key{ elem.key() };
                if (elem.type() == bsoncxx::type::k_utf8) {
                    std::cout << "key = " << elem.key() << " , value = " << elem.get_string().value << std::endl;
                    return std::string(elem.get_string().value);
                }
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB exception caught: " << e.what() << std::endl;
        assert(false);
    }

}
void mongo::SetIpAddressOfServer(const char* IP_Address)
{
    try {
        mongocxx::instance inst{};
        std::string tttt = "mongodb+srv://kumaranish987654:jxiXLao2l8RZ90FE@cluster0.xkhsevv.mongodb.net/";
        const auto uri = mongocxx::uri{ "mongodb+srv://kumaranish987654:jxiXLao2l8RZ90FE@cluster0.xkhsevv.mongodb.net/" };

        mongocxx::options::client client_options;
        const auto api = mongocxx::options::server_api{ mongocxx::options::server_api::version::k_version_1 };
        client_options.server_api_opts(api);

        mongocxx::client conn{ uri, client_options };
        mongocxx::database db = conn["userdata"];

        const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        db.run_command(ping_cmd.view());
        std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;

        auto collection = db["IP"];

        bsoncxx::builder::stream::document filter{};
        filter << "_id" << bsoncxx::oid("65e5f0eb4eedd387f93ee1c9");

        bsoncxx::builder::stream::document update{};
        update << "$set" << bsoncxx::builder::stream::open_document
            << "server_id" << IP_Address
            << bsoncxx::builder::stream::close_document;

        collection.update_one(filter.view(), update.view());

        std::cout << "Document updated successfully!" << std::endl;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB exception caught: " << e.what() << std::endl;
        //assert(false);
    }
}
