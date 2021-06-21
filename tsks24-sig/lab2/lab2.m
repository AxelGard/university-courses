clear;
close all;

% 3  Förberedelseuppgifter
%  En station ̈ar sinussignal med frekvens 4 kHz samplas och rekonstrueras sedan idealt.
%  Vilken frekvens får resultatet om samplingsfrekvensen ̈ar
%   a. 10 kHz?
%       4 kHz, because 10/2 = 5 > 4 => ideal rekonstruktion helt.
%   b. 5 kHz?
%       1 kHz, if 4 < fs < 4*2 => fres = fs - 4 = 1
%   c. 3 kHz?
%       1 kHz, look at google doc for calculation
%

% 4.1  initial calculation
T = 2.5;
fs = 4e4;
N = T * fs;
n = 0:N-1;
t = (1/fs)*n;
f1 = 8000;
f2 = 8017;
x1 = sin(2*pi*f1*t);
x2 = sin(2*pi*f2*t);

% length(x1) % - get amount of sampels
% length(x2)

figure('Name', '4.1 plot graph');
subplot(1, 2, 1);
plot(t, x1, 'b-', t , x1, 'rx');
title('x1');
xlabel('t');
ylabel('x1(t)');
xlim([0 (1/fs)*25]); % 5 period a 5 each
subplot(1, 2, 2);
plot(t, x2, 'b-', t , x2, 'rx');
title('x2');
xlabel('t');
ylabel('x2(t)');
xlim([0 (1/fs)*25]); % 5 period a 5 each

figure('Name', '4.1 histograms');
subplot(1, 2, 1);
histogram(x1, 100);
title('x1');
xlabel('x1(t)');
ylabel('amount of sampels');
subplot(1, 2, 2);
histogram(x2, 100);
title('x2');
xlabel('x2(t)');
ylabel('amount of sampels');

%{
how many sampels does the signal consist of?
  20k * 5 = 100_000 sampel

How many periods does the signal consist of?
  x1: 20k, 100000 / 5
  x2: 20k, 100000 / (40000 / 8017) = 20042.5

How many sapels does a period consist of?
  x1: 5
  x2: 5, 40000 / 8017

Does the signal period even out on any number of sampels?
    x1: Period = 1 / 8000 = 0.000125, This means that we need to sample
      in a multiple of 8000 for it to even out.
    x2: Period = 1 / 8017 = 0.0001247..., This means that we need to sample
      in a multiple of 8017 for it to even out.

what does the histogram show?
    For x1 the histogram shows that the sampels are grouped into 5 different bins.
    For x2 the histogram shows that the sampels are spread out across the spectrum.

Why are they different?
    They are different because the sampling frequency 40000 is an even multiple of
    8000 meaning it will sample x1 evenly. But it is not an even multiple of 8017
    therefore it will not sample evenly, which means sampels will be spread across the spectrum.

%}



% 4.2 Spectrum

f = (fs/N) * n;

x1_fft = abs(fft(x1));
x2_fft = abs(fft(x2));
figure('Name', '4.2 Spectrum');
subplot(1,2,1);
plot(f, x1_fft);
title('x1');
xlabel('Natural frequency (f) [Hz] ');
ylabel('|F(x1)|');
subplot(1,2,2);
plot(f, x2_fft);
title('x2');
xlabel('Natural frequency (f) [Hz] ');
ylabel('|F(x2)|');

%{
    Explain what is visible in the plots:
        We see the signals amplitudekarakteristic which has a peak at the signals frequency.

    Why are there two tops?
        Z(f) = j/2(S(f + fs) - S(f - fs)). Repeated in at each multiple of f.
        f = 0 => impulse at -fs and +fs => -8000, 8000
        f = 1 => impulse at f-fs and f+fs => 40000 - 8000, 40000 + 8000 = 32000, 48000
        we see impulse at 8000 and 32000 for x1
        for x1 we see impulse at 8017 and 40000-8017 = 31983

    how much of these spectrum are relevant to show?
        The second top is not really relevant as it is implied by the first one.
%}

x1_db = db(x1_fft);
x2_db = db(x2_fft);
figure('Name', '4.2 dB');
subplot(1,2,1);
plot(f, x1_db);
title('x1');
xlabel('Natural frequency (f) [Hz] ');
ylabel('|F(x1)| [dB]');
subplot(1,2,2);
plot(f, x2_db);
title('x2');
xlabel('Natural frequency (f) [Hz] ');
ylabel('|F(x2)| [dB]');

%{
    Explain what is visible in the plots:
        In the plot for x1_db we see that the difference between the tops and the
        other points are much larger than in the plot for x2_db.
        What looks like noice in x1_db is most likely just rounding errors cause by matlab.

    There is a clear difference between the two plots. Why is that?
        The difference is because of the difference in signal frequency which means
        that when sampling x2 we find different frequencies because of the signal frequency
        and the sample frequency not being in sync.

%}


% 4.3 Window functions

M = 50;
w_50 = [zeros(1,floor((N-M)/2)),rectwin(M)',zeros(1,ceil((N-M)/2))];
M = 200;
w_200 = [zeros(1,floor((N-M)/2)),rectwin(M)',zeros(1,ceil((N-M)/2))];

figure('Name', '4.3 Window functions (rectwin)');
subplot(1,2,1);
x = db(abs(fft(w_50)));
x = x - max(x);
plot(x);
title('Rectwin(50)');
xlabel('t');
ylabel('|F(rectwin(50))| (0 = max) [dB]');
subplot(1,2,2);
x = db(abs(fft(w_200)));
x = x - max(x);
plot(x);
title('Rectwin(200)');
xlabel('t');
ylabel('|F(rectwin(200))| (0 = max) [dB]');


M = 50;
w_50 = [zeros(1,floor((N-M)/2)),nuttallwin(M)',zeros(1,ceil((N-M)/2))];
M = 200;
w_200 = [zeros(1,floor((N-M)/2)),nuttallwin(M)',zeros(1,ceil((N-M)/2))];

figure('Name', '4.3 Window functions (nuttallwin)');
subplot(1,2,1);
x = db(abs(fft(w_50)));
x = x - max(x);
plot(x);
title('Nuttallwin(50)');
xlabel('t');
ylabel('|F(nuttallwin(50))| (0 = max) [dB]');
subplot(1,2,2);
x = db(abs(fft(w_200)));
x = x - max(x);
plot(x);
title('Nuttallwin(200)');
xlabel('t');
ylabel('|F(nuttallwin(200))| (0 = max) [dB]');

%{
how wide is the main lobe for the given M value
    rectwin 50: 4k in length
    rectwin 200: 1k in length

    nuttallwin 50: 16k in length
    nuttallwin 200: 4k in length


how wide is the side-lobes for the given M value
  rectwin 50: 2k in length
  rectwin 200: 500 in length

  nuttallwin 50: 2k in length
  nuttallwin 200: 500 in length

difference between largest side-lobe and main-lobe in dB.
    rectwin 50: 13.25
    rectwin 200: 13.25

    nuttallwin 50: 92.2
    nuttallwin 200: 97.7

Difference between the windows:
    nuttallwin creates a much larger difference between the main-lobe and the side-lobes
    when compared to rectwin.

%}



% 4.4 Windowing

M = 500;
rectwin_500 = [zeros(1,floor((N-M)/2)),rectwin(M)',zeros(1,ceil((N-M)/2))];
nuttallwin_500 = [zeros(1,floor((N-M)/2)),nuttallwin(M)',zeros(1,ceil((N-M)/2))];
M = 2000;
rectwin_2000 = [zeros(1,floor((N-M)/2)),rectwin(M)',zeros(1,ceil((N-M)/2))];
nuttallwin_2000 = [zeros(1,floor((N-M)/2)),nuttallwin(M)',zeros(1,ceil((N-M)/2))];

x = sin(2*pi*3999*(1+5e-7*cos(2*pi*100*t)).*t)+sin(2*pi*4099*t)+1e-3*sin(2*pi*4400*t);

figure('Name', '4.4 Windowing (x)');
plot(x);
title('x');
xlabel('t');
ylabel('x(t)');
xlim([0 800]);

x_fft = abs(fft(x));

x_rect_500 = rectwin_500 .* x;
x_rect_500_fft = abs(fft(x_rect_500));
figure('Name', '4.4 Windowing (rect_500 amplitudespectrum)');
plot(f, db(x_rect_500_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, rect(500), amplitudespectrum');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*rect(500))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);


x_nuttallwin_500 = nuttallwin_500 .* x;
x_nuttallwin_500_fft = abs(fft(x_nuttallwin_500));
figure('Name', '4.4 Windowing (nuttallwin_500 amplitudespectrum)');
plot(f, db(x_nuttallwin_500_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, nuttallwin(500), amplitudespectrum');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*nuttallwin(500))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

x_rect_2000 = rectwin_2000 .* x;
x_rect_2000_fft = abs(fft(x_rect_2000));
figure('Name', '4.4 Windowing (rect_2000 amplitudespectrum)');
plot(f, db(x_rect_2000_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, rect(2000), amplitudespectrum');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*rect(2000))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

x_nuttallwin_2000 = nuttallwin_2000 .* x;
x_nuttallwin_2000_fft = abs(fft(x_nuttallwin_2000));
figure('Name', '4.4 Windowing (nuttallwin_2000 amplitudespectrum)');
plot(f, db(x_nuttallwin_2000_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, nuttallwin(2000), amplitudespectrum');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*nuttallwin(2000))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

%{

How good is the different cases at revise the orginal spectrum?
    rectwin_500: Quite bad, you can see something in the middle but not make out that there are two tops.
    rectwin_2000: Better, it is now possible to make out the two tops but the side-lobes are quite high.
    nuttallwin_500: Quite bad, you can see something in the middle but not make out that there are two tops.
    nuttallwin_2000: The best in our opinion, as you can make out the two tops clearly but also the smaller tops to the side.

Why is this?
    rectwin_500: 500 is quite a small window making it hard to get a good result.
        Also, for rect the difference between the main-lobe and the side-lobes is quite small
        making it hard to distinguish between them.
    rectwin_2000: This good because it has a larger window but the problem with rect that
        the side-lobes are quite high still exist, making it hard to find the smaller tops.
    nuttallwin_500: 500 is quite a small window making it hard to get a good result. This
        in combination with the large main-lobe causes the each small top to generate a large
        lobe in the windowed version, and all these large lobes combine to create one big hump.
    nuttallwin_2000: This is good because of the large window and also the fact that nuttallwin
        has a large difference beween the side-lobes and the main-lobe, meaning it is easy
        to find the even the smaller tops.

Is there any details from the orginal spectrum that is not shown in the window case?
    rectwin_500: You can not see that there are two tops.
    rectwin_2000: It is hard to see the small tops to the side of the two larger tops.
    nuttallwin_500: You can not see that there are two tops.
    nuttallwin_2000: No.

Is there any details that apper in the window case that is not shown in the orginal spectrum?
    rectwin_500: No.
    rectwin_2000: No.
    nuttallwin_500: No.
    nuttallwin_2000: There seems to be a small bump at 3800 Hz.

%}


x_rect_500 = rectwin_500 .* x;
x_rect_500 = x_rect_500(49751:50250);
x_rect_500_fft = abs(fft(x_rect_500));
figure('Name', '4.4 Windowing (rect_500 amplitudespectrum windowed sampels only)');
f_ = f(1 : (N/500) : end); % pick 500 evenly spaced values from f
plot(f_, db(x_rect_500_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, rect(500), amplitudespectrum windowed sampels only');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*rect(500))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

x_rect_2000 = rectwin_2000 .* x;
x_rect_2000 = x_rect_2000(49001:51000);
x_rect_2000_fft = abs(fft(x_rect_2000));
figure('Name', '4.4 Windowing (rect_2000 amplitudespectrum windowed sampels only)');
f_ = f(1 : (N/2000) : end); % pick 2000 evenly spaced values from f
plot(f_, db(x_rect_2000_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, rect(2000), amplitudespectrum windowed sampels only');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*rect(2000))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

x_nuttallwin_500 = nuttallwin_500 .* x;
x_nuttallwin_500 = x_nuttallwin_500(49751:50250);
x_nuttallwin_500_fft = abs(fft(x_nuttallwin_500));
figure('Name', '4.4 Windowing (nuttallwin_500 amplitudespectrum windowed sampels only)');
f_ = f(1 : (N/500) : end); % pick 500 evenly spaced values from f
plot(f_, db(x_nuttallwin_500_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, nuttallwin(500), amplitudespectrum windowed sampels only');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*nuttallwin(500))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);

x_nuttallwin_2000 = nuttallwin_2000 .* x;
x_nuttallwin_2000 = x_nuttallwin_2000(49001:51000);
x_nuttallwin_2000_fft = abs(fft(x_nuttallwin_2000));
figure('Name', '4.4 Windowing (nuttallwin(2000) amplitudespectrum windowed sampels only)');
f_ = f(1 : (N/2000) : end); % pick 2000 evenly spaced values from f
plot(f_, db(x_nuttallwin_2000_fft));
hold on;
plot(f, db(x_fft));
title('Windowing, nuttallwin(2000), amplitudespectrum windowed sampels only');
xlabel('Natural frequency (f) [Hz]');
ylabel('|F(x)| [dB]');
legend(["|F(x*nuttallwin(2000))|", "|F(x)|"]);
hold off;
xlim([3000, 5000]);


%{

In a practical application we would only use the windowed sampels and not
the sampels which equate to zero. And after that calculate the FFT. Do this for
the six? cases.
Does this change the situation for the worse?

rectwin_500: Slightly worse.
rectwin_2000: Almost an improvement, easier to find the smaller spikes.
nuttallwin_500: Slightly worse.
nuttallwin_2000: Almost an improvement, but bery similar.


%}
