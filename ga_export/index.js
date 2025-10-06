const protobuf = require("protobufjs");
const fs = require('fs');
const base32 = require('hi-base32');

async function decodeMessage(buffer) {
    const root = await protobuf.load("migration-payload.proto");
    const payload = root.lookupType("MigrationPayload");
    const err = payload.verify(buffer);
    if (err) {
        throw err;
    }
    const message = payload.decode(buffer);
    const obj = payload.toObject(message);

    return payload.toObject(message);
}

async function printOTPCodes(otpBuffer) {
    const payload = await decodeMessage(otpBuffer);

    const otpArray = payload.otpParameters;
    for(let i = 0; i < otpArray.length; i++) {
        const otp = otpArray[i];
        const base32Encoded = base32.encode(otp.secret)
        console.log("Issuer: " + otp.issuer);
        console.log("Name: " + otp.name);
        console.log("Secret: " + base32Encoded);
        console.log("Secret as Hex: " + otp.secret);
        let hexDecoded = Buffer.from(otp.secret, 'ascii').toString('hex')
        let hexDecodedArr = [];
        for (i=0; i < hexDecoded.length-1; i+=2) {
            hexDecodedArr.push(`0x${hexDecoded[i].toLocaleLowerCase()}${hexDecoded[i+1].toLocaleLowerCase()}`);
        }
        
        console.log(`Secret as Hex array: ${hexDecodedArr.join(", ")}`);
        console.log("-----------------------------------");
    }
}

console.log("=== DECODING ===\n");
console.log(process.argv[2]);
const url = new URL(process.argv[2]);
const otpBuffer = Buffer.from(url.searchParams.get('data'), 'base64');
printOTPCodes(otpBuffer).catch(err => console.error(err));