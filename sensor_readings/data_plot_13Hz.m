%% Data analysis

clear all;
clc;
close all;

%% Plot unfiltered data at 0000g

unf_0000 = importdata("unfiltered_0000g_13Hz.txt");
x = [1:1:length(unf_0000)];

figure();
plot(x, unf_0000, "yellow");
title("Magnetometer reading at 0000g");

%% Plot mean

meanall = mean(unf_0000)
hold on
plot(x, meanall*ones(length(unf_0000),1), "blue");

%% plot mean - by 20 numbers per mean calculation

mean1_20 = mean(unf_0000(1:20))
plot(x(1:20), mean1_20 *ones(20,1), "green");

mean1_40 = mean(unf_0000(20:40))
plot(x(20:40), mean1_40 *ones(21,1), "green");

mean1_60 = mean(unf_0000(40:60))
plot(x(40:60), mean1_60 *ones(21,1), "green");

mean1_80 = mean(unf_0000(60:80))
plot(x(60:80), mean1_80 *ones(21,1), "green");

mean1_100 = mean(unf_0000(1:20))
plot(x(80:100), mean1_100 *ones(21,1), "green");

%% Plot mean - by 100 numbers per mean calculation

mean2_100 = mean(unf_0000(1:100))
plot(x(1:100), mean2_100*ones(100,1), "m");
mean2_200 = mean(unf_0000(100:200))
plot(x(100:200), mean2_200*ones(101,1), "m");
mean2_300 = mean(unf_0000(200:300))
plot(x(200:300), mean2_300*ones(101,1), "m");
mean2_400 = mean(unf_0000(300:400))
plot(x(300:400), mean2_400*ones(101,1), "m");

%% Plot mean - by 200 numbers per mean calculation

mean3_200 = mean(unf_0000(1:200))
plot(x(1:200), mean3_200*ones(200,1), "k");
mean3_400 = mean(unf_0000(200:400))
plot(x(200:400), mean3_200*ones(201,1), "k");
