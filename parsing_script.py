import ast
import csv

log_names = ['1.844m', '1.111m', '0.352m', '0.394m', '0.826m']


def parse_packet(data_string, frame_start, frame_stop):
    frame_strings = data_string.strip().split('\n')
    frames = []
    for frame_string in frame_strings:
        frame_list = ast.literal_eval(frame_string)
        frames.append(frame_list)

    packet_address = frames[1][5]
    packet_data = ''
    for frame in frames:
        if len(packet_data) == 0:
            packet_data += frame[7]
        else:
            packet_data += frame[7][2:]
    parsed_packet = f'Packet from {frame_start} to {frame_stop} is destined to {packet_address} with {packet_data} data\n'
    if parsed_packet.find('0x80B0') != -1:
        parsed_packet = ('=======================================================================\n'
                         '=========================MEASUREMENT '
                         'DELIMITER=========================\n=======================================================================\n') + parsed_packet
    return parsed_packet


def log_compression(input_file_name, output_file_name, one_line_print=True):
    log_file = open(input_file_name, 'r')
    compressed_log_file = open(output_file_name, 'w')

    first_line = True

    for line in log_file:
        data = line.split()
        try:
            data = data[10]
            if data != '0x80B0' and first_line:
                continue
            if data == '0x80B0' and first_line:
                first_line = False
            elif data == '0x80B0' and not first_line and one_line_print:
                return
            else:
                compressed_log_file.write('\t')
            compressed_log_file.write(str(data))
        except:
            ...


def parse_csv(input_csv_file_name, output_parsing_file_name):
    input_csv_file = open(input_csv_file_name, mode='r')
    output_parsing_file = open(output_parsing_file_name, 'w')

    csv_reader = csv.reader(input_csv_file)

    frame_start = 0
    frame = ''
    row_index = 0

    for row in csv_reader:
        if row[1] == 'start':
            frame_start = row_index
            frame += str(row) + '\n'
        elif row[1] == 'stop':
            frame += str(row) + '\n'
            parsed_packet = parse_packet(frame, frame_start, row_index)
            output_parsing_file.write(parsed_packet)
            frame = ''
        elif row[0] != 'name':
            frame += str(row) + '\n'

        row_index = row_index + 1
        # print(row)

    input_csv_file.close()
    output_parsing_file.close()

def log_diff(log_name_list, output_file_name):
    log_name_list = [f"{name}_compressed.txt" for name in log_name_list]

    try:
        files = [open(file_name, 'r') for file_name in log_name_list]

        with open(output_file_name, 'w', newline='') as output_file:
            csv_writer = csv.writer(output_file)
            for lines in zip(*[file.readlines() for file in files]):
                stripped_lines = [line.rstrip('\n').replace('\t', ' ') for line in lines]

                if not all(len(line) == len(stripped_lines[0]) for line in stripped_lines):
                    raise ValueError("Lines in files are not of the same length.")
                result = ''.join(
                    char if all(char == other_line[i] for other_line in stripped_lines)
                    else '='
                    for i, char in enumerate(stripped_lines[0])
                )

                for substring in result.split():
                    csv_writer.writerow([substring])

    except FileNotFoundError as e:
        print(f"Error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        for file in files:
            file.close()

if __name__ == "__main__":
    for log_name in log_names:
        log_name_csv = log_name + '.csv'
        log_name_parsed = log_name + '_parsed.log'
        log_name_compressed = log_name + '_compressed.txt'

        parse_csv(log_name_csv, log_name_parsed)
        log_compression(log_name_parsed, log_name_compressed)
    log_diff(log_names, 'diff_file.csv')
