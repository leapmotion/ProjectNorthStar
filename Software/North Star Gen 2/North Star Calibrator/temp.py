def processData(data, polynomialDegree=3):
    """Fit a 2D polynomial using data, mapping from Screen UVs to either Camera Pixel or Rectilinear Coordinates"""
    # Prepare the mask of valid measurements
    mask = createMask(data)
    data *= mask[...,None]

    # Extract the Valid Measurements and arrange into a flat array
    coordinates          = np.zeros((data.shape[0], data.shape[1], 2), dtype=np.int)  
    coordinates[:, :, 0] = np.arange(0, data.shape[0])[:, None]  # Prepare the coordinates
    coordinates[:, :, 1] = np.arange(0, data.shape[1])[None, :]  # Prepare the coordinates

    # Sample the non-zero indices from the flattened coordinates and data arrays
    non_zero_indices     = np.nonzero(mask.reshape(-1))[0] # Get non-zero mask indices
    non_zero_data = data[..., 1:].reshape(-1, 2)[non_zero_indices].astype(np.float) / 255

    non_zero_pixel_coordinates = coordinates.reshape(-1, 2)[non_zero_indices].astype(np.float)

    pxc_x = non_zero_pixel_coordinates[:,0]
    pxc_y = non_zero_pixel_coordinates[:,1]

    ray_coordinates_x = []
    ray_coordinates_y = []

    for x_pix,y_pix in zip(pxc_x, pxc_y):
        x_ray = (x_pix-500)/500
        y_ray = (y_pix-500)/500
        ray_coordinates_x.append(x_ray)
        ray_coordinates_y.append(y_ray)
    ray_coordinates_x = np.array(ray_coordinates_x)
    ray_coordinates_y = np.array(ray_coordinates_y)
      # Fit the multidimensional polynomials
    x_coeffs = polyfit2d(non_zero_data[:, 1],        # "Input"  X Axis
                                             non_zero_data[:, 0],        # "Input"  Y Axis
                                             ray_coordinates_x, # "Output" X Axis
                                             np.asarray([polynomialDegree,
                                                                     polynomialDegree]))

    y_coeffs = polyfit2d(non_zero_data[:, 1],        # "Input"  X Axis
                                             non_zero_data[:, 0],        # "Input"  Y Axis
                                             ray_coordinates_y, # "Output" Y Axis
                                             np.asarray([polynomialDegree,
                                                                     polynomialDegree]))

    # Print the Calibration Coefficients
    print_coeffs(x_coeffs)
    print_coeffs(y_coeffs)

    # Reconstruct the data as if it came from the polynomial
    X, Y           = np.meshgrid(       (np.arange(0.0, 300.0)/300.0), 
                                                                1.0 - (np.arange(0.0, 533.0)/533.0))
    fitted         = np.zeros((533, 300, 3), dtype=np.float)
    fitted[..., 2] = polyval2d(X, Y, x_coeffs)
    fitted[..., 1] = polyval2d(X, Y, y_coeffs)

    return fitted