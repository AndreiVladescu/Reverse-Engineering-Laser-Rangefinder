import ast
import csv


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

def log_compression():
    log_file = open('parsed_data.log', 'r')
    compressed_log_file = open('compressed_parsed_data.log', 'w')

    lines_skipped = 0
    print_compression = False
    last_line = ''
    first_line = True

    for line in log_file:

        if line.find('0xC000') != -1:
            lines_skipped = lines_skipped + 1
            if lines_skipped == 1 and print_compression:
                compressed_log_file.write('...')
        else:
            if lines_skipped > 0:
                if print_compression:
                    compressed_log_file.write(' x ' + str(lines_skipped) + '\n' + last_line)
                lines_skipped = 0
            data = line.split()
            try:
                data = data[10]
                if data == '0x80B0' and first_line:
                    compressed_log_file.write('\n')
                else:
                    compressed_log_file.write('\t')
                compressed_log_file.write(str(data))
            except:
                ...

            last_line = line

def parse_csv():
    input_csv_file = open('data_i2c.csv', mode='r')
    output_parsing_file = open('parsed_data.log', 'w')

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
            # print(f'Frame starts at {frame_start} and stops at {row_index}:\n{frame}')
            frame = ''
        elif row[0] != 'name':
            frame += str(row) + '\n'

        row_index = row_index + 1
        # print(row)

    input_csv_file.close()
    output_parsing_file.close()


if __name__ == "__main__":
    #parse_csv()
    log_compression()