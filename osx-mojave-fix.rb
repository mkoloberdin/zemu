#!/usr/bin/ruby

#   typedef int     cpu_type_t;
#   typedef int     cpu_subtype_t;
#
#   struct mach_header_64 {
#       uint32_t        magic;          /* mach magic number identifier */
#       cpu_type_t      cputype;        /* cpu specifier */
#       cpu_subtype_t   cpusubtype;     /* machine specifier */
#       uint32_t        filetype;       /* type of file */
#       uint32_t        ncmds;          /* number of load commands */
#       uint32_t        sizeofcmds;     /* the size of all the load commands */
#       uint32_t        flags;          /* flags */
#       uint32_t        reserved;       /* reserved */
#   };
#
#   struct load_command {
#       uint32_t cmd;       /* type of load command */
#       uint32_t cmdsize;   /* total size of command in bytes */
#   };

class MachO
    MH_MAGIC = 0xfeedfacf
    LC_BUILD_VERSION = 0x32

    def initialize(path)
        @path = path
        @ptr = 0

        File.open(path, 'rb') { |f| @data = f.read }
    end

    def find_load_command(cmd_to_find)
        @ptr = 0

        raise 'Mach-O magic not found' unless next_int == MH_MAGIC
        @ptr += 3 * 4 # skip "cputype", "cpusubtype" and "filetype"

        ncmds = next_int
        @ptr += 3 * 4 # skip "sizeofcmds", "flags" and "reserved"

        for i in 1 .. ncmds do
            cmd_ptr = @ptr
            cmd = next_int

            if cmd == cmd_to_find
                return cmd_ptr
            end

            cmdsize = next_int
            @ptr = cmd_ptr + cmdsize
        end

        return nil
    end

    def next_int
        result = @data[@ptr].ord + (@data[@ptr + 1].ord << 8) + (@data[@ptr + 2].ord << 16) + (@data[@ptr + 3].ord << 24)
        @ptr += 4
        return result
    end

    def put_int(ptr, value)
        @data[ptr] = (value & 0xff).chr
        @data[ptr + 1] = ((value >> 8) & 0xff).chr
        @data[ptr + 2] = ((value >> 16) & 0xff).chr
        @data[ptr + 3] = ((value >> 24) & 0xff).chr
    end

    def write_out
        File.open(@path, 'wb') { |f| f << @data }
    end

    def perform_mojave_fix
        lc_build_version_ptr = find_load_command(LC_BUILD_VERSION)

        unless lc_build_version_ptr.nil?
            put_int(lc_build_version_ptr, 0)
            write_out
        end
    end
end

if ARGV.size != 1
    puts "Usage: ruby #{__FILE__} <executable-to-fix>"
else
    MachO.new(ARGV[0]).perform_mojave_fix
end
