"""This file contains important algorithms for Falcon.

- the Fast Fourier orthogonalization (in coefficient and FFT representation)
- the Fast Fourier nearest plane (in coefficient and FFT representation)
- the Fast Fourier sampling (only in FFT)
.
"""
from falcon.common import split, merge                         # Split, merge
from falcon.fft import add, sub, mul, div, adj                 # Operations in coef.
from falcon.fft import add_fft, sub_fft, mul_fft, div_fft, adj_fft  # Ops in FFT
from falcon.fft import split_fft, merge_fft, fft_ratio         # FFT
from falcon.sampler import sampler_z                           # Gaussian sampler in Z
from falcon.newsampler import samplerz

def gram(B):
    """Compute the Gram matrix of B.

    Args:
        B: a matrix

    Format: coefficient
    """
    rows = range(len(B))
    ncols = len(B[0])
    deg = len(B[0][0])
    G = [[[0 for coef in range(deg)] for j in rows] for i in rows]
    for i in rows:
        for j in rows:
            for k in range(ncols):
                G[i][j] = add(G[i][j], mul(B[i][k], adj(B[j][k])))
    return G


def ldl(G):
    """Compute the LDL decomposition of G.

    Args:
        G: a Gram matrix

    Format: coefficient

    Corresponds to algorithm 8 (LDL) of Falcon's documentation,
    except it's in polynomial representation.
    """
    deg = len(G[0][0])
    dim = len(G)
    L = [[[0 for k in range(deg)] for j in range(dim)] for i in range(dim)]
    D = [[[0 for k in range(deg)] for j in range(dim)] for i in range(dim)]
    for i in range(dim):
        L[i][i] = [1] + [0 for j in range(deg - 1)]
        D[i][i] = G[i][i]
        for j in range(i):
            L[i][j] = G[i][j]
            for k in range(j):
                L[i][j] = sub(L[i][j], mul(mul(L[i][k], adj(L[j][k])), D[k][k]))
            L[i][j] = div(L[i][j], D[j][j])
            D[i][i] = sub(D[i][i], mul(mul(L[i][j], adj(L[i][j])), D[j][j]))
    return [L, D]


def ldl_fft(G):
    """Compute the LDL decomposition of G.

    Args:
        G: a Gram matrix

    Format: FFT

    Corresponds to algorithm 8 (LDL) of Falcon's documentation.
    """
    deg = len(G[0][0])
    dim = len(G)
    L = [[[0 for k in range(deg)] for j in range(dim)] for i in range(dim)]
    D = [[[0 for k in range(deg)] for j in range(dim)] for i in range(dim)]
    for i in range(dim):
        L[i][i] = [1 for j in range(deg)]
        D[i][i] = G[i][i]
        for j in range(i):
            L[i][j] = G[i][j]
            for k in range(j):
                L[i][j] = sub_fft(L[i][j], mul_fft(mul_fft(L[i][k], adj_fft(L[j][k])), D[k][k]))
            L[i][j] = div_fft(L[i][j], D[j][j])
            D[i][i] = sub_fft(D[i][i], mul_fft(mul_fft(L[i][j], adj_fft(L[i][j])), D[j][j]))
    return [L, D]


def ffldl(G):
    """Compute the ffLDL decomposition tree of G.

    Args:
        G: a Gram matrix

    Format: coefficient

    Corresponds to algorithm 9 (ffLDL) of Falcon's documentation,
    except it's in polynomial representation.
    """
    n = len(G[0][0])
    L, D = ldl(G)
    # Coefficients of L, D are elements of R[x]/(x^n - x^(n/2) + 1), in coefficient representation
    if (n > 2):
        # A bisection is done on elements of a 2*2 diagonal matrix.
        d00, d01 = split(D[0][0])
        d10, d11 = split(D[1][1])
        G0 = [[d00, d01], [adj(d01), d00]]
        G1 = [[d10, d11], [adj(d11), d10]]
        return [L[1][0], ffldl(G0), ffldl(G1)]
    elif (n == 2):
        # Bottom of the recursion.
        D[0][0][1] = 0
        D[1][1][1] = 0
        return [L[1][0], D[0][0], D[1][1]]


def ffldl_fft(G):
    """Compute the ffLDL decomposition tree of G.

    Args:
        G: a Gram matrix

    Format: FFT

    Corresponds to algorithm 9 (ffLDL) of Falcon's documentation.
    """
    n = len(G[0][0]) * fft_ratio
    L, D = ldl_fft(G)
    # Coefficients of L, D are elements of R[x]/(x^n - x^(n/2) + 1), in FFT representation
    if (n > 2):
        # A bisection is done on elements of a 2*2 diagonal matrix.
        d00, d01 = split_fft(D[0][0])
        d10, d11 = split_fft(D[1][1])
        G0 = [[d00, d01], [adj_fft(d01), d00]]
        G1 = [[d10, d11], [adj_fft(d11), d10]]
        return [L[1][0], ffldl_fft(G0), ffldl_fft(G1)]
    elif (n == 2):
        # End of the recursion (each element is real).
        return [L[1][0], D[0][0], D[1][1]]


def ffnp(t, T):
    """Compute the ffnp reduction of t, using T as auxilary information.

    Args:
        t: a vector
        T: a ldl decomposition tree

    Format: coefficient
    """
    n = len(t[0])
    z = [None, None]
    if (n > 1):
        l10, T0, T1 = T
        z[1] = merge(ffnp(split(t[1]), T1))
        t0b = add(t[0], mul(sub(t[1], z[1]), l10))
        z[0] = merge(ffnp(split(t0b), T0))
        return z
    elif (n == 1):
        z[0] = [round(t[0][0])]
        z[1] = [round(t[1][0])]
        return z


def ffnp_fft(t, T):
    """Compute the ffnp reduction of t, using T as auxilary information.

    Args:
        t: a vector
        T: a ldl decomposition tree

    Format: FFT
    """
    n = len(t[0]) * fft_ratio
    z = [0, 0]
    if (n > 1):
        l10, T0, T1 = T
        z[1] = merge_fft(ffnp_fft(split_fft(t[1]), T1))
        t0b = add_fft(t[0], mul_fft(sub_fft(t[1], z[1]), l10))
        z[0] = merge_fft(ffnp_fft(split_fft(t0b), T0))
        return z
    elif (n == 1):
        z[0] = [round(t[0][0].real)]
        z[1] = [round(t[1][0].real)]
        return z


def ffsampling_fft(t, T):
    """Compute the ffsampling of t, using T as auxilary information.

    Args:
        t: a vector
        T: a ldl decomposition tree

    Format: FFT
    Corresponds to algorithm 11 (ffSampling) of Falcon's documentation.
    """
    n = len(t[0]) * fft_ratio
    z = [0, 0]
    if (n > 1):
        l10, T0, T1 = T
        z[1] = merge_fft(ffsampling_fft(split_fft(t[1]), T1))
        t0b = add_fft(t[0], mul_fft(sub_fft(t[1], z[1]), l10))
        z[0] = merge_fft(ffsampling_fft(split_fft(t0b), T0))
        return z
    elif (n == 1):
        z[0] = [sampler_z(T[0], t[0][0].real)]
        z[1] = [sampler_z(T[0], t[1][0].real)]
        # z[0] = [samplerz(t[0][0].real, T[0])]
        # z[1] = [samplerz(t[1][0].real, T[0])]
        return z
