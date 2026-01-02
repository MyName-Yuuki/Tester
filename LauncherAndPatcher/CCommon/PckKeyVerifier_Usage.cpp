// ===== CONTOH PENGGUNAAN PCK KEY VERIFIER =====
// File ini menunjukkan cara pakai di kode yang sudah ada

#include "PckKeyVerifier.h"
#include <AFilePackage.h>
#include <AFilePackMan.h>

// ============================================
// CONTOH 1: BUKA PCK DENGAN VERIFIKASI KEY
// ============================================

void Example1_OpenPckWithKey()
{
    // STEP 1: Set key rahasia (SEKALI saja di awal)
    PCK_SetKey("MY-SECRET-KEY-12345");

    // STEP 2: Verify key SEBELUM buka file
    if (!PCK_VerifyKey("configs.pck"))
    {
        printf("ERROR: Key salah! Tidak bisa buka configs.pck\n");
        return;
    }

    // STEP 3: Key benar, buka file biasa
    g_AFilePackMan.OpenFilePackageInGame("configs.pck");

    // File terbuka!
}

// ============================================
// CONTOH 2: PROSES HANYA PCK YANG ADA
// ============================================

void Example2_ProcessAvailablePck()
{
    // Set key
    PCK_SetKey("MY-SECRET-KEY-12345");

    // List PCK yang mau diproses
    const char* pckFiles[] = {
        "configs.pck",
        "gfx.pck",
        "models.pck",
        "textures.pck",
        "interfaces.pck"
    };

    // Proses yang ADA saja (flexible)
    DWORD dwProcessed = PCK_ProcessMultiple(pckFiles, 5);

    printf("Berhasil memproses %d file PCK\n", dwProcessed);
}

// ============================================
// CONTOH 3: DECOMPRESS DARI PCK DENGAN KEY
// ============================================

void Example3_DecompressFromPck()
{
    // Set key
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Baca data dari PCK
    AFileImage file;
    file.Open("", "configs\\data.bin", AFILE_OPENEXIST | AFILE_BINARY);

    DWORD dwCompSize = file.GetFileLength();
    BYTE* pCompData = new BYTE[dwCompSize];
    file.Read(pCompData, dwCompSize);
    file.Close();

    // Dekompresi DENGAN VERIFIKASI KEY
    BYTE* pOutput = new BYTE[dwCompSize * 10];
    DWORD dwOutSize;

    if (!PCK_DecompressWithKey(pCompData, dwCompSize, "configs.pck",
                                pOutput, &dwOutSize))
    {
        printf("ERROR: Gagal dekompresi (key salah?)\n");
        delete[] pCompData;
        delete[] pOutput;
        return;
    }

    // Berhasil! Data di pOutput
    delete[] pCompData;
    delete[] pOutput;
}

// ============================================
// CONTOH 4: COMPRESS UNTUK PCK DENGAN KEY
// ============================================

void Example4_CompressForPck()
{
    // Set key
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Data yang mau dikompresi
    BYTE pData[1000];
    // ... isi data ...

    // Kompresi DENGAN KEY
    BYTE* pCompressed = new BYTE[2000];
    DWORD dwCompSize;

    if (!PCK_CompressWithKey(pData, 1000, "configs.pck",
                              pCompressed, &dwCompSize))
    {
        printf("ERROR: Gagal kompresi\n");
        delete[] pCompressed;
        return;
    }

    // Data siap ditulis ke PCK
    delete[] pCompressed;
}

// ============================================
// CONTOH 5: PATCH/UPDATE PCK DENGAN KEY
// ============================================

void Example5_PatchPck()
{
    // Set key
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Data baru untuk patch
    BYTE pNewData[1000];
    // ... isi data baru ...

    // Patch ke configs.pck
    if (!PCK_PatchWithKey("configs.pck", "configs\\data.bin",
                           pNewData, 1000))
    {
        printf("ERROR: Patch gagal (key salah?)\n");
        return;
    }

    printf("Patch berhasil!\n");
}

// ============================================
// CONTOH 6: INTEGRASI DENGAN KODE YANG SUDAH ADA
// ============================================

// GANTI KODE YANG SUDAH ADA:

// ----- KODE LAMA (Tanpa Key) -----
void OldCode_NoKey()
{
    // Langsung buka PCK
    g_AFilePackMan.OpenFilePackageInGame("configs.pck");

    // Decompress
    AFilePackage::Uncompress(data, size, output, &outsize);
}

// ----- KODE BARU (Dengan Key) -----
void NewCode_WithKey()
{
    // Set key dulu
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Verify dulu
    if (!PCK_VerifyKey("configs.pck"))
    {
        // Error handling
        return;
    }

    // Baru buka
    g_AFilePackMan.OpenFilePackageInGame("configs.pck");

    // Decompress dengan key
    PCK_DecompressWithKey(data, size, "configs.pck", output, &outsize);
}

// ============================================
// CONTOH 7: UPDATE BAGIAN TERTENTU SAJA
// ============================================

void Example7_UpdateSpecificPck()
{
    // Set key
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Update hanya configs.pck
    if (PCK_HasKeyProtection("configs.pck"))
    {
        if (!PCK_VerifyKey("configs.pck"))
        {
            printf("Key salah untuk configs.pck!\n");
            return;
        }

        // Lanjut update configs.pck
        printf("Updating configs.pck...\n");
        // ... update code ...
    }
    else
    {
        // configs.pck tidak diproteksi, update biasa
        printf("Updating configs.pck (no key)...\n");
        // ... update code ...
    }
}

// ============================================
// CONTOH 8: FLEXIBLE - PROSES SESUAI YANG ADA
// ============================================

void Example8_FlexibleProcess()
{
    PCK_SetKey("MY-SECRET-KEY-12345");

    // Coba configs.pck
    if (PCK_VerifyKey("configs.pck"))
    {
        printf("configs.pck: Key OK, memproses...\n");
        g_AFilePackMan.OpenFilePackageInGame("configs.pck");
    }
    else
    {
        printf("configs.pck: Key salah atau file tidak ada, skip\n");
    }

    // Coba gfx.pck
    if (PCK_VerifyKey("gfx.pck"))
    {
        printf("gfx.pck: Key OK, memproses...\n");
        g_AFilePackMan.OpenFilePackageInGame("gfx.pck");
    }
    else
    {
        printf("gfx.pck: Key salah atau file tidak ada, skip\n");
    }

    // ... dan seterusnya
}
