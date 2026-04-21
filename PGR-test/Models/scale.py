input_filename = 'light_switch.obj'
output_filename = 'light_switch_scaled.obj'
scale_factor = 2.0  # Adjust this value to get the exact size you need

with open(input_filename, 'r') as f_in, open(output_filename, 'w') as f_out:
    for line in f_in:
        if line.startswith('v '):
            parts = line.split()
            # Scale X, Y, and Z coordinates
            x, y, z = [float(p) * scale_factor for p in parts[1:4]]
            f_out.write(f"v {x} {y} {z}\n")
        else:
            f_out.write(line)


        