/*
Node: /map_server
URI: rosrpc://aditya-ubuntu-20:53505
Type: nav_msgs/LoadMap
Args: map_url
*/

#include "ros/ros.h"
#include "nav_msgs/LoadMap.h"
#include <rospack/rospack.h>
#include <yaml-cpp/yaml.h>

using namespace std;

int main(int argc, char **argv)
{

  /***** GET PACKAGE PATH FUNCTION => not needed any more ******/

  // int ret;
  // bool quiet;
  // try
  // {
  //   rospack::ROSPack rp;
  //   try
  //   {
  //     int arg_count = 3;
  //     char *arg_vector[arg_count] = {"prohibitor", "find", package};
  //     ret = rp.run(arg_count, arg_vector);
  //     package_path = rp.getOutput();
  //     package_path.erase(std::remove(package_path.begin(), package_path.end(), '\n'), package_path.end());
  //   }
  //   catch (std::runtime_error &e)
  //   {
  //     ret = -1;
  //     if (!rp.is_quiet())
  //       throw;
  //   }
  // }
  // catch (std::runtime_error &e)
  // {
  //   fprintf(stderr, "%s\n", e.what());
  //   ret = -1;
  // }

  // if (ret)
  //   exit(1);

  /***** ***** ******/

  string map_name = argv[1];
  string map_path;
  map_path = map_name + ".yaml";
  cout << "Map Path: " << map_path << endl;

  /***** READ POLYGON.YAML ******/

  /***** ***** ******/

  /***** READ MAP_FILTER.YAML ******/

  YAML::Node config = YAML::LoadFile(map_path);

  // if (config.IsNull())
  // {
  //   cerr << "Error: Could not open or parse YAML file." << endl;
  //   return 1;
  // }

  // string image = config["name"].as<std::string>();
  const string username = config["image"].as<string>();

  // std::cout << "occupied_thresh: " << occupied_thresh << std::endl;

  //   // Access elements in the YAML file
  //   int occupied_thresh = config["occupied_thresh"].as<int>();

  //   // Output the values
  //   cout << "occupied_thresh: " << occupied_thresh << endl;
  // }
  // catch (const exception &e)
  // {
  //   cerr << "Error: " << e.what() << endl;
  //   return 1;
  // }

  /***** ***** ******/

  /***** UPDATE MAP_FILTER ******/

  /***** ***** ******/

  /***** CALL SERVICE ******/

  ros::init(argc, argv, "prohibitor");

  ros::NodeHandle nh;
  ros::ServiceClient client = nh.serviceClient<nav_msgs::LoadMap>("change_map");

  nav_msgs::LoadMap srv;
  srv.request.map_url = map_path;

  if (client.call(srv))
  {

    ROS_INFO("Called service successfully");
  }
  else
  {
    ROS_ERROR("Failed to call service");
    return 1;
  }

  /***** ***** ******/

  return 0;
}