import numpy as np

def BetterSpecAnal(img:np.ndarray, n_windows: int=25, window_height: int=64, window_width: int=64)-> np.ndarray:
    """
    BetterSpecAnal(img, n_windows, window_height, window_width)
    Performs the estimate of the Power Spectral Density (psd) of the input img via block averaging as described here:
    -    https://engineering.purdue.edu/~bouman/ece637/notes/pdf/RandProc.pdf
    Author: Joseph K. Kulikowski

    Parameters
    ----------
    img : numpy.ndarray
        Image to perform the estimate of the Power Spectral Density of
    n_windows : int=25
        Number of windows to perform block averaging by. Blocks will be taken out of img in a
        int(sqrt(n_windows))xint(sqrt(n_windows)) grid around the center of the image.
    window_height : int=64
        Number of rows in each window.
    window_width : int=64
        Number of columns in each window
    
    Returns
    -------
    psd : numpy.ndarray
        The estimate of the power spectral density of the image.

    """
    # I decide that this will be a sqrt(n_windows)xsqrt(n_windows) set of blocks starting at the center pixel.

    im_height, im_width = np.size(img, 0), np.size(img, 1)

    # Since our image is even shaped in both height and width, I will define the "Center" of the image to be 
    # 1/2 pixels left and up from the true center of the image, which is between 4 pixels.
    center_y, center_x = im_height//2, im_width//2

    # allocate a 3D array for our window_height x window_width x n_windows to perform fft/averaging over.
    windows = np.zeros(shape=(window_height, window_width, n_windows))*np.nan
    # make all NaN for a sanity check assertion after assignment.

    # Counter for array sizes
    count = 0
    # define each window by 2D hamming window as described.
    W = np.outer(np.hamming(window_height), np.hamming(window_width))

    for iwindow_y in range(-int(np.sqrt(n_windows))//2 + 1, int(np.sqrt(n_windows))//2 + 1):
        for iwindow_x in range(-int(np.sqrt(n_windows))//2 + 1, int(np.sqrt(n_windows))//2 + 1):
            new_center_y = center_y + (iwindow_y * window_height//2)
            new_center_x = center_x + (iwindow_x * window_width//2)
            y_start = new_center_y - window_height//2
            y_end = new_center_y + window_height//2
            x_start = new_center_x - window_width//2
            x_end = new_center_x + window_width//2

            # populate our windows array with our image in each window
            windows[:,:,count] = img[y_start:y_end, x_start:x_end]
            # Ensure we've assigned all the elements in this slice.
            assert(~np.any(np.isnan(windows[:,:,count])))
            # Multiply each window by 2D hamming window as described.
            operated_on = W * windows[:,:,count]
            # Take 2D FFT, shift to -pi,pi, square it
            operated_on = np.fft.fftshift(np.abs(np.fft.fft2(operated_on)))**2
            windows[:,:,count] = operated_on
            # push our count up
            count = count + 1

    # We now have our 25 windows to average over.
    assert(~np.any(np.isnan(windows)))
    psd = np.sum(windows, 2)/np.size(windows, 2)
    return psd


        