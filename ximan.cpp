#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

std::string USERNAME;
const std::string CORE_REPO = "https://github.com/xi816/ximan-core/";
const std::string CORE_FILE = "/home/" + USERNAME + "/.config/ximan/core.ximan";

std::string getUsername() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw) {
    return std::string(pw->pw_name);
  }
  return "";
}

int coreSearch(const std::string &core) { return 0; }

std::unordered_map<std::string, std::vector<std::string>> ximanReadCore() {
  std::unordered_map<std::string, std::vector<std::string>> core;
  std::ifstream fcore(CORE_FILE);
  if (!fcore.is_open()) {
    std::cerr << "Error: " << CORE_FILE << " not found." << std::endl;
    return core;
  }

  std::string line;
  while (std::getline(fcore, line)) {
    if (!line.empty()) {
      std::istringstream iss(line);
      std::string key, url, version;
      if (iss >> key >> url >> version) {
        core[key] = {url, version};
      }
    }
  }
  return core;
}

int ximanUpdate() {
  std::string yn;
  std::cout << "Proceed with update? [Y/n]: ";
  std::getline(std::cin, yn);
  if (yn.empty())
    yn = "y";
  if (yn == "1")
    return 1;
  if (yn == "y" || yn == "yes" || yn == "") {
    std::system(("rm -rf /home/" + USERNAME + "/.config/ximan/").c_str());
    std::system(
        ("git clone " + CORE_REPO + " /home/" + USERNAME + "/.config/ximan/")
            .c_str());
    std::system(("rm -rf /home/" + USERNAME + "/.config/ximan/.git/").c_str());
  } else {
    return 1;
  }
  return 0;
}

int ximanInstall(const std::vector<std::string> &prog, bool upgrade = false) {
  bool toHome = false;
  std::string yn;
  std::cout << "Proceed with installation? [Y/n]: ";
  std::getline(std::cin, yn);
  if (yn.empty())
    yn = "y";

  auto core = ximanReadCore();
  if (prog[0] == "-h") {
    toHome = true;
  }

  if (yn == "y" || yn == "yes" || yn == "") {
    for (const auto &pr : prog) {
      if (core.find(pr) != core.end()) {
        std::cout << "  Installing " << pr << "..." << std::endl;
        if (toHome) {
          if (upgrade) {
            std::system(
                ("sudo rm -rf /home/" + USERNAME + "/" + pr + "/").c_str());
          }
          std::system(
              ("sudo mkdir -p /home/" + USERNAME + "/" + pr + "/").c_str());
          std::system(("sudo git clone " + core[pr][0] + " /home/" + USERNAME +
                       "/" + pr + "/")
                          .c_str());
          std::system(("sudo chown " + USERNAME + " -R /home/" + USERNAME +
                       "/" + pr + "/")
                          .c_str());
        } else {
          std::system("sudo mkdir -p /usr/share/ximan/");
          if (upgrade) {
            std::system(("sudo rm -rf /usr/share/ximan/" + pr + "/").c_str());
          }
          std::system(("sudo git clone " + core[pr][0] + " /usr/share/ximan/" +
                       pr + "/")
                          .c_str());
          std::system(
              ("sudo chown " + USERNAME + " -R /usr/share/ximan/" + pr + "/")
                  .c_str());
        }
      } else {
        std::cout << "Package " << pr << " not found" << std::endl;
        return 1;
      }
    }
  } else {
    return 1;
  }
  return 0;
}

std::string join(const std::vector<std::string> &vec,
                 const std::string &delim) {
  std::ostringstream oss;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i != 0)
      oss << delim;
    oss << vec[i];
  }
  return oss.str();
}

int ximanMain(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "ximan: no arguments given" << std::endl;
    return 1;
  }

  std::vector<std::string> args(argv + 1, argv + argc);
  if (args[0] == "update") {
    return ximanUpdate();
  } else if (args[0] == "install") {
    if (args.size() < 2) {
      std::cout << "ximan: no packages given" << std::endl;
      return 1;
    }
    std::cout << "Installing packages:" << std::endl;
    if (args[1] != "-h") {
      std::cout << "  \033[32m"
                << join(std::vector<std::string>(args.begin() + 1, args.end()),
                        " ")
                << "\033[0m" << std::endl;
    } else {
      std::cout << "  \033[32m"
                << join(std::vector<std::string>(args.begin() + 2, args.end()),
                        " ")
                << "\033[0m" << std::endl;
    }
    return ximanInstall(std::vector<std::string>(args.begin() + 1, args.end()));
  } else if (args[0] == "upgrade") {
    if (args.size() < 2) {
      std::cout << "ximan: no packages given" << std::endl;
      return 1;
    }
    std::cout << "Upgrading packages:" << std::endl;
    if (args[1] != "-h") {
      std::cout << "  \033[32m"
                << join(std::vector<std::string>(args.begin() + 1, args.end()),
                        " ")
                << "\033[0m" << std::endl;
    } else {
      std::cout << "  \033[32m"
                << join(std::vector<std::string>(args.begin() + 2, args.end()),
                        " ")
                << "\033[0m" << std::endl;
    }
    return ximanInstall(std::vector<std::string>(args.begin() + 1, args.end()),
                        true);
  } else {
    std::cout << "ximan: unknown command '" << args[0] << "'" << std::endl;
    return 1;
  }
}

int main(int argc, char *argv[]) {
  USERNAME = getUsername();
  return ximanMain(argc, argv);
}