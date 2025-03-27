import serial
import time
import json
from djitellopy import Tello

with open("demo_settings.json", "r") as file:
    config = json.load(file)

# Assign values to variables
DRONE_IP = config["drone_ip"]
ADD_FLIP = config["add_flip"]




class Nacelle_controller:
    def __init__(self, baudrate):
        #GPIO 14 (TX) and GPIO 15 (RX)
        self.grabber_status = [0,0,0,0]
        self.baudrate = baudrate
        time.sleep(2)
        self.actions = {
                "status": [self.get_nacelle_status,0,"Asks and waits for a status update frome the nacelle."],
                "reset": [self.reset_nacelle,0,"Reset the pi pico on the nacelle, closing all grabbing modules."],
                "token_demo": [self.token_demo,4,"A useless token to demonstrate how the CMD terminal and tokens work together."],
                "load" : [self.load_nacelle,1,"Loads grabber modules. Takes the target grabber id as a parameter, waits for confirmation and a list of the load grabber modules"],
                "open" : [self.open, 1,"Opens grabber module. Takes the target grabber id "],
                "close" : [self.close, 1,"Closes grabber module. Takes the target grabber id "],
                "force_cycle" : [self.open, 1,"Unloads grabber module. Takes the target grabber id"],
                "force_open" : [self.force_open, 0,"Opens all grabbing modules"],
                "force_close" : [self.force_close, 0,"Closes all grabbing modules"],
                "help" : [self.help,0,"Prints a list of commands and their parameters"],
                "demo" : [self.demo,1,"Demo program to use after a drone is loaded and ready to drop"],
                "hello" : [self.hello_world,0,"Says hello to the nacelle"]



        }

    def send_serial_message(self,message,timeout=10):
        try:
            with serial.Serial('/dev/serial0', self.baudrate,timeout=timeout) as ser: 
                time.sleep(0.1)  # Small delay to stabilize connection
                ser.write(message.encode())  # Send message
                start_time = time.time()  # Record start time
                response = ""
                while time.time() - start_time < timeout:
                    chunk = ser.readline().decode('utf-8').strip()  # Read response line
                    if chunk:
                        response += chunk
                        break  # Stop reading once a response is received
                return response if response else None  # Return received response or None
        except serial.SerialException as e:
            print(f"Serial error: {e}")
     

    def demo(self, id):
        tello = Tello(host=DRONE_IP)
        
        try:
            tello.connect()
        except Exception as e:
            print(f"Error connecting to the drone: {e}")
            return
        
        print("takeoff")
        try:
            tello.initiate_throw_takeoff()
            time.sleep(1)
            self.open(id)
            
            # Start flight
            tello.move_forward(30)
            if ADD_FLIP:
                tello.flip_forward()
                tello.flip_back()
            
            tello.land()
        except Exception as e:
            print(f"Error during flight: {e}")
        finally:
            # Disconnect
            tello.end()



    def hello_world(self):
        reponse = self.send_serial_message("Hello nacelle")
        print("Nacelle : " + reponse)

    def load_nacelle(self,target): #done
        msg = ""
        if(int(target)> 0 and int(target) < 5):
            msg = f"load {target}\n"
        else:
            return print("target set failed, out of range or incorrect input")
        response = self.send_serial_message(msg,timeout=20)
        if response == "failed to load":
            print("Failed to load, took more than the given time")
        else:
           print(f" Grabber {target} loaded.")
            

    def token_demo(self,arg1,arg2,arg3,arg4):
        print("Good job you use the demo token correctly !\n")
        print(f"You add the following arguments to the token : {arg1}, {arg2}, {arg3}, {arg4} !\n")
        return
    
    def open(self,id):#done
        reponse = self.send_serial_message("open " + str(id))
        print("grabber " + str(id) + " openned.") if reponse == "open_success" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return

    def close(self,id):#done
        reponse = self.send_serial_message("close " + str(id))
        print("grabber " + str(id) + " closed.") if reponse == "close_success" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return

    def force_open(self): #done
        reponse = self.send_serial_message("force_open")
        print("all grabbers openned.") if reponse == "open_forced" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return

        
    def force_close(self): #done
        reponse = self.send_serial_message("force_closed")
        print("all grabbers close.") if reponse == "close_forced" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")        
        return

    def reset_nacelle(self): #done
        response = self.send_serial_message("reset_req")
        print("RESET CONFIRMED") if response == "reset confirmation" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return
            
    def get_nacelle_status(self): #done
        response = self.send_serial_message("status_req")
        print(response)
        print(response) if response is not None else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return    
        
    def help(self):
        print("Available Actions:\n")
        for token, details in self.actions.items():
            function, param_count, description = details
            param_info = "No parameters" if param_count == 0 else f"{param_count} parameter(s) expected"
            print(f"Token: '{token}'\n  Description: {description}\n  Parameters: {param_info}\n")
        return
        
        
    def check_action(self,action):
        if action in self.actions:
            return True
        else :
            return False
        
    def send_action(self):
        cmd = input('Enter a command TOKEN and the required arguments :\n')
        parts = cmd.split()  # Split command into parts
        command_name = parts[0] 
        if command_name in self.actions:
            func, param_count,_= self.actions[command_name]
            args = parts[1:param_count+1] 
            if len(args) == param_count: 
                return func(*args)
            else:
                return print(f"Error: '{command_name}' expects {param_count} arguments, but got {len(args)}.")
        else:
            return "Error: Command not found."
                
                
nacelle = Nacelle_controller(115200)
while(1):
    nacelle.send_action()

        
        
        