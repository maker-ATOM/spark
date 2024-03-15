#!/usr/bin/env python3

import sys
import rospy
from nav_msgs.srv import LoadMap, LoadMapRequest
import yaml
from PIL import Image, ImageDraw


if __name__ == "__main__":
    
    # Get path to map and .yaml file
    map_name = sys.argv[1]
    map_path = map_name + ".yaml"
    polygon_path = sys.argv[2]
    
    rospy.init_node("prohibitor")
    
    # Read Polygon.yaml
    with open(polygon_path, 'r') as file:
        polygon_yaml = yaml.safe_load(file)

    # Read map.yaml
    with open(map_path, 'r') as file:
        map_yaml = yaml.safe_load(file)
    
    # Get origin of map frame
    map_origin = map_yaml["origin"]
    resolution = map_yaml["resolution"]
     
    # Read map file
    with open(map_name + ".pgm", 'rb') as f:
        
        format = f.readline().decode().strip()
        
        while True:
            line = f.readline().decode().strip()

            # Skip comments
            if line.startswith('#'):
                continue
            else:
                break
            
        # Get metadata
        width, height = map(int, line.split())
        max_gray = int(f.readline().decode().strip())

        if format == 'P5':
            # Get 1D list of image
            map_file = list(f.read())
        else:
            raise ValueError("Unsupported PGM format: {}".format(format))
        
    # Convert to 2D array for better manipulability
    map_image = Image.frombytes('L', (width, height), bytes(map_file))
       
    # Add Remove sector based on yaml file
    draw = ImageDraw.Draw(map_image)
    
    # Iterate through each sector
    for key, value in polygon_yaml.items():
        
        if polygon_yaml[key]["use"]:
            action = "Removing"
        else:
            action = "Adding"
        rospy.loginfo(f"{action} {key} with coordinates {polygon_yaml[key]['coordinates']}")
        
        # Covert the coordinates from map frame to image frame
        coordinate_list = [((x_m - map_origin[0]) / resolution, ((map_origin[1] - y_m) / resolution) + height) for x_m, y_m in polygon_yaml[key]["coordinates"]]

        # ------- DRAW THROUGH ITERATING OVER EACH COORDINATE PAIR ------- #
        
        # Iterate trough each coordinate pair 
        # for i in range(0, len(coordinate_list)):
            
        #     # Edit map with lines based on to add or remove the lines
        #     shape = [tuple(coordinate_list[i]), tuple(coordinate_list[(i + 1) % len(coordinate_list)])]
        #     draw = ImageDraw.Draw(map_image)   
        #     draw.line(shape, fill = 255 * polygon_yaml[key]["use"], width = 2)
        
        # ------- DRAW USING POLYGON METHOD ------- #
        draw.polygon(coordinate_list, fill = 255 * polygon_yaml[key]["use"])
                
    # Viz modified image
    # map_image.show()
    
    # Convert to 1D list again
    modified_map = list(map_image.getdata())
    
    # Over write the previous map file 
    with open(map_name + ".pgm", 'wb') as f:
        
        f.write("P5\n".encode())
        f.write("{} {}\n".format(width, height).encode())
        f.write("{}\n".format(max_gray).encode())

        f.write(bytes(modified_map))
    
    # Call Service
    
    rospy.loginfo("Waiting for service...")
    rospy.wait_for_service("/change_map")
    rospy.loginfo("Service available...")
    
    try:
        service = rospy.ServiceProxy("/change_map", LoadMap)

        request = LoadMapRequest()
        request.map_url = map_path

        response = service(request)
        
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)