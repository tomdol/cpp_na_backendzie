const express = require('express');
const fs = require('fs');
const app = express();

app.get('/', (req, res) => {
    const path = "/home/tomek/test_img.bmp"; // obrazek 12MB, 12MB, 2000x2000px
    const scale = req.query.scale || 5;

    openAndResize(path, scale)
        .then(resized_img => {
            res.send(`${resized_img.length}`);
        });
});

const openAndResize = function(path, scale) {
    return new Promise((resolve, reject) => {
        fs.readFile(path, (err, imgData) => {
            if (err) {
                reject(err);
            } else {
                const resized = resize(imgData, scale);
                resolve(resized);
            }
        });
    });
}

const resize = function(img, scale) {
    const resized_img = [];

    for (let i = 0; i < scale; ++i) {
        for (let j = 0; j < img.length; ++j) {
            resized_img.push(img[j]);
        }
    }

    return resized_img;
}

console.log('Running on port:', process.argv[2] || 8888);

app.listen(process.argv[2] || 8888);
