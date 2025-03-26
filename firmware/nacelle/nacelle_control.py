import serial
import time

class Nacelle_controller:
    def __init__(self, baudrate):
        #GPIO 14 (TX) and GPIO 15 (RX)
        self.used_grabbers = 1
        self.grabber_status = [0,0,0,0]
        self.baudrate = baudrate
        time.sleep(2)
        self.actions = {
                "status": [self.get_nacelle_status,0,"Asks and waits for a status update frome the nacelle."],
                "reset": [self.reset_nacelle,0,"Reset the pi pico on the nacelle, closing all grabbing modules."],
                "token_demo": [self.token_demo,4,"A useless token to demonstrate how the CMD terminal and tokens work together."],
                "load" : [self.load_nacelle,1,"Loads grabber modules. Takes the target grabber id as a parameter or \"ALL\" to load all, waits for confirmation and a list of the load grabber modules"],
                "open" : [self.open, 1,"Unloads grabber module. Takes the target grabber id or \"ALL\""],
                "force_open" : [self.force_open, 1,"Opens all grabbing modules"],
                "force_close" : [self.force_close, 1,"Closes all grabbing modules"],
                "help" : [self.help,0,"Prints a list of commands and their parameters"]


        }

    def send_serial_message(self,message,timeout=5):
        try:
            with serial.Serial('/dev/serial0', self.baudrate,timeout=timeout) as ser:  # Set read timeout to 1s
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
     
    def load_nacelle(self,target):
        msg = ""
        if(target == "all"):
            msg = "load all"
        elif(int(target)> 0 and target <= self.used_grabbers):
            msg = "load {target}"
        else:
            return print("target set failed, out of range or incorrect input")
        response = self.send_serial_message(msg)
        if response is not None:
            print("failed to set")
        else:
            self.grabber_status = response.split()
            


    def token_demo(self,arg1,arg2,arg3,arg4):
        print("Good job you use the demo token correctly !\n")
        print(f"You add the following arguments to the token : {arg1}, {arg2}, {arg3}, {arg4} !\n")
        return
    
    def open(self,id):
        reponse = self.send_serial_message("open " + str(id))
        print("grabber " + str(id) + " openned.") if reponse == "open_1" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return

    def force_open(self,id):
        reponse = self.send_serial_message("open " + str(id))
        print("all grabbers openned.") if reponse == "open_forced" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return

        
    def force_close(self,id):
        reponse = self.send_serial_message("open " + str(id))
        print("all grabbers close.") if reponse == "open_forced" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")        
        return

    def reset_nacelle(self):
        response = self.send_serial_message("reset_req")
        print("RESET CONFIRMED") if response == "reset confirmation" else print("COMMUNCATION FAILED, CHECK PHYSICAL CONNECTION")
        return
            
    def get_nacelle_status(self):
        response = self.send_serial_message("status_req")
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
                
                
nacelle = Nacelle_controller(115520)
nacelle.send_action()

        
        
        