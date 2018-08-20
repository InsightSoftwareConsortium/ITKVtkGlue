FROM dockcross/manylinux-x64:latest

# VTK requirements
RUN gosu root yum install -y libXt-devel mesa-libGL-devel

ENV DEFAULT_DOCKCROSS_IMAGE insighttoolkit/itkvtkglue
