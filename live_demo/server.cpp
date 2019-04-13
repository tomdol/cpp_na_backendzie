#include <pistache/endpoint.h>
#include <vector>
#include <fstream>
#include <iterator>

using namespace Pistache;

class ResizeHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(ResizeHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override{
        const auto scale_from_req = request.query().get("scale").getOrElse("5");

        const auto path = "/home/tomek/test_img.bmp"; // obrazek bmp, 12MB, 2000x2000px
        const size_t scale = std::stoul(scale_from_req);

        const std::vector<char> resized_img = open_and_resize(path, scale);

        response.send(Pistache::Http::Code::Ok, std::to_string(resized_img.size()));
    }

private:
    std::vector<char> open_img(const std::string& path) {
        std::ifstream img_file{path, std::ios::binary};

        return std::vector<char>{
            std::istreambuf_iterator<char>{img_file}, 
            std::istreambuf_iterator<char>{}
        };
    }

    std::vector<char> open_and_resize(const std::string& path, size_t scale) {
        const auto img = open_img(path);

        const size_t resized_img_size{img.size() * scale};

        std::vector<char> resized_img{};
        resized_img.reserve(resized_img_size);

        for (size_t i = 0; i < scale; ++i) {
            for (size_t j = 0; j < img.size(); ++j) {
                resized_img.push_back(img[j]);
            }
        }

        return resized_img;
    }

};

int main(int argc, char** argv) {
    auto port = 9999;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));

    auto opts = Pistache::Http::Endpoint::options()
        .threads(1); // 1 vs 4 vs 8

    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<ResizeHandler>());
    server.serve();

    server.shutdown();
}
