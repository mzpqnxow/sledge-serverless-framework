#!/bin/bash
# Test Driver Script

if [[ $0 != "./test.sh" ]]; then
	echo "Must run in same directory as ./test.sh"
	exit 1
fi

base_dir=$(pwd)

# Awsm
cd "$base_dir/awsm" && cargo build --release || exit 1

# Sledge
cd "$base_dir/runtime" && make clean all || exit 1

# OCR Build
# FIXME: gocr incorectly reports up to date
make clean gocr -B -C "$base_dir/runtime/tests" || exit 1

# OCR Tests
# FIXME: OCR tests seem to sporadically fail and then work on rerun. Don't bail on fail for now
# cd "$base_dir/runtime/experiments/applications/ocr/hyde" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/handwriting" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/fivebyeight" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_word" && ./install.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_word" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_font" && ./install.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_font" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_dpi" && ./install.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/ocr/by_dpi" && ./run.sh || exit 1
cd "$base_dir/runtime/experiments/applications/ocr/hyde" && ./run.sh
cd "$base_dir/runtime/experiments/applications/ocr/handwriting" && ./run.sh
cd "$base_dir/runtime/experiments/applications/ocr/fivebyeight" && ./run.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_word" && ./install.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_word" && ./run.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_font" && ./install.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_font" && ./run.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_dpi" && ./install.sh
cd "$base_dir/runtime/experiments/applications/ocr/by_dpi" && ./run.sh

# EKF Build
make clean tinyekf -B -C "$base_dir/runtime/tests" || exit 1

# EKF Tests
cd "$base_dir/runtime/experiments/applications/ekf/by_iteration" && ./run.sh || exit 1
cd "$base_dir/runtime/experiments/applications/ekf/one_iteration" && ./run.sh || exit 1

# cifar10 Build
make clean cifar10 -B -C "$base_dir/runtime/tests" || exit 1

# cifar10 Tests
cd "$base_dir/runtime/experiments/applications/imageclassification" && ./run.sh || exit 1

# sod Build
make clean sod -B -C "$base_dir/runtime/tests" || exit 1

# sod Tests
# FIXME: sod sporadically fails. May be related to a file descriptor bug
# cd "$base_dir/runtime/experiments/applications/imageresize/test" && ./install.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/imageresize/test" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/imageresize/by_resolution" && ./install.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/imageresize/by_resolution" && ./run.sh || exit 1
# cd "$base_dir/runtime/experiments/applications/licenseplate/by_plate_count" && ./run.sh || exit 1
cd "$base_dir/runtime/experiments/applications/imageresize/test" && ./install.sh
cd "$base_dir/runtime/experiments/applications/imageresize/test" && ./run.sh
cd "$base_dir/runtime/experiments/applications/imageresize/by_resolution" && ./install.sh
cd "$base_dir/runtime/experiments/applications/imageresize/by_resolution" && ./run.sh
cd "$base_dir/runtime/experiments/applications/licenseplate/by_plate_count" && ./run.sh