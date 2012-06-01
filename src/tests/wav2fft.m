arg_list = argv();

files = ls(arg_list{1});
disp(['running on ' int2str(size(files,1)) ' files'])

for i = 1:(size(files,1))
    filename = strtrim(files(i,:));
    disp(['loading file ''' filename ''''])
    [wave, fs, bits] = wavread(filename);    % load wave audio with length and sample rate

    N   = fs; %length(wave);
    f   = 0:1/N*fs:(N-1)/N*fs;                  % frequency vector

    disp('applying fft...')
    ftwave = fft(wave(1:fs));             % do a fourier transform of wave

    disp('plotting...')
    h=figure('Visible', 'off');                % Bildschirmplot
    plot(f(1:(fs/2.0)),20*log10(abs( ftwave(1:(fs/2.0)) )) - 90);         % Betragsspektrum x-Vektor
    xlabel('f/Hz');
    ylabel('dB');

    print(h,'-dpng',[filename '.png']);
endfor
