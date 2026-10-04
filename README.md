# Ball-sandbox-n64
This is a small little fun sandbox game I created in C++ and originally ported from the Windows XP version I targeted. To compile it, you need libdragon to compile n64 libraries

(Clone the library and complete installation steps from their github page)

git clone https://github.com/DragonMinded/libdragon.git

(Set up environment variables)


echo 'export N64_INST=/opt/libdragon' >> ~/.bashrc

echo 'export PATH=$PATH:$N64_INST/bin' >> ~/.bashrc

source ~/.bashrc

(build tool-chain and compile libraries)

sudo mkdir -p /opt/libdragon

sudo chown -R $(whoami) /opt/libdragon

cd ~/libdragon-source

./tools/build-toolchain.sh

(Finish and compile)

make

make install

(Install using git clone)

git clone https://github.com/jaw201106/Ball-sandbox-n64.git

(open folder and compile assuming libdragon went accordingly)

echo 'export N64_INST=/opt/libdragon' >> ~/.bashrc

echo 'export PATH=$PATH:$N64_INST/bin' >> ~/.bashrc

make
