// ============================================================================
// generador_audio.cpp - Generador de sonidos 8-bit para Z-Ware
// Genera archivos WAV usando solo C++ estándar (sin dependencias externas)
// ============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdint>

// ============================================================================
// CONSTANTES
// ============================================================================
constexpr int SAMPLE_RATE = 22050;
constexpr int NUM_CHANNELS = 1;
constexpr int BIT_DEPTH = 8;

// ============================================================================
// ESTRUCTURAS WAV
// ============================================================================
struct RIFFHeader {
    char riff[4] = {'R', 'I', 'F', 'F'};
    uint32_t fileSize;
    char wave[4] = {'W', 'A', 'V', 'E'};
};

struct FMTChunk {
    char fmt[4] = {'f', 'm', 't', ' '};
    uint32_t chunkSize = 16;
    uint16_t audioFormat = 1;  // PCM
    uint16_t numChannels = NUM_CHANNELS;
    uint32_t sampleRate = SAMPLE_RATE;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample = BIT_DEPTH;
};

struct DATAChunk {
    char data[4] = {'d', 'a', 't', 'a'};
    uint32_t dataSize;
};

// ============================================================================
// FUNCIONES DE GENERACIÓN DE ONDAS
// ============================================================================

// Genera onda cuadrada (classic 8-bit sound)
std::vector<int8_t> generateSquareWave(float frequency, float duration, float volume) {
    int numSamples = static_cast<int>(SAMPLE_RATE * duration);
    std::vector<int8_t> samples;
    samples.reserve(numSamples);
    
    for (int i = 0; i < numSamples; ++i) {
        float t = static_cast<float>(i) / SAMPLE_RATE;
        float value = (std::sin(2.0f * M_PI * frequency * t) > 0.0f) ? 1.0f : -1.0f;
        int sample = 128 + static_cast<int>(value * volume * 127.0f);
        samples.push_back(static_cast<int8_t>(std::min(255, std::max(0, sample)) - 128));
    }
    return samples;
}

// Genera onda sawtooth (más áspero)
std::vector<int8_t> generateSawtoothWave(float frequency, float duration, float volume) {
    int numSamples = static_cast<int>(SAMPLE_RATE * duration);
    std::vector<int8_t> samples;
    samples.reserve(numSamples);
    
    for (int i = 0; i < numSamples; ++i) {
        float t = static_cast<float>(i) / SAMPLE_RATE;
        float value = 2.0f * std::fmod(frequency * t, 1.0f) - 1.0f;
        int sample = 128 + static_cast<int>(value * volume * 127.0f);
        samples.push_back(static_cast<int8_t>(std::min(255, std::max(0, sample)) - 128));
    }
    return samples;
}

// Genera ruido blanco
std::vector<int8_t> generateNoise(float duration, float volume) {
    int numSamples = static_cast<int>(SAMPLE_RATE * duration);
    std::vector<int8_t> samples;
    samples.reserve(numSamples);
    
    for (int i = 0; i < numSamples; ++i) {
        float value = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        int sample = 128 + static_cast<int>(value * volume * 127.0f);
        samples.push_back(static_cast<int8_t>(std::min(255, std::max(0, sample)) - 128));
    }
    return samples;
}

// Aplica envolvente ADSR
std::vector<int8_t> applyEnvelope(const std::vector<int8_t>& input, 
                                   float attack, float decay, 
                                   float sustain, float release) {
    int numSamples = input.size();
    std::vector<int8_t> output;
    output.reserve(numSamples);
    
    int attackSamples = static_cast<int>(SAMPLE_RATE * attack);
    int decaySamples = static_cast<int>(SAMPLE_RATE * decay);
    int releaseSamples = static_cast<int>(SAMPLE_RATE * release);
    int sustainSamples = numSamples - attackSamples - decaySamples - releaseSamples;
    
    if (sustainSamples < 0) sustainSamples = 0;
    
    int idx = 0;
    
    // Attack
    for (int i = 0; i < attackSamples && idx < numSamples; ++i, ++idx) {
        float env = static_cast<float>(i) / attackSamples;
        int val = 128 + static_cast<int>((input[idx] + 128) * env);
        output.push_back(static_cast<int8_t>(std::min(255, std::max(0, val)) - 128));
    }
    
    // Decay
    for (int i = 0; i < decaySamples && idx < numSamples; ++i, ++idx) {
        float env = 1.0f - (static_cast<float>(i) / decaySamples) * (1.0f - sustain);
        int val = 128 + static_cast<int>((input[idx] + 128) * env);
        output.push_back(static_cast<int8_t>(std::min(255, std::max(0, val)) - 128));
    }
    
    // Sustain
    for (int i = 0; i < sustainSamples && idx < numSamples; ++i, ++idx) {
        int val = 128 + static_cast<int>((input[idx] + 128) * sustain);
        output.push_back(static_cast<int8_t>(std::min(255, std::max(0, val)) - 128));
    }
    
    // Release
    for (int i = 0; i < releaseSamples && idx < numSamples; ++i, ++idx) {
        float env = sustain * (1.0f - static_cast<float>(i) / releaseSamples);
        int val = 128 + static_cast<int>((input[idx] + 128) * env);
        output.push_back(static_cast<int8_t>(std::min(255, std::max(0, val)) - 128));
    }
    
    return output;
}

// Mezcla dos señales
std::vector<int8_t> mixSamples(const std::vector<int8_t>& a, const std::vector<int8_t>& b, float ratioA, float ratioB) {
    size_t maxLen = std::max(a.size(), b.size());
    std::vector<int8_t> result;
    result.reserve(maxLen);
    
    for (size_t i = 0; i < maxLen; ++i) {
        int sA = (i < a.size()) ? (a[i] + 128) : 128;
        int sB = (i < b.size()) ? (b[i] + 128) : 128;
        int mixed = static_cast<int>(sA * ratioA + sB * ratioB);
        result.push_back(static_cast<int8_t>(std::min(255, std::max(0, mixed)) - 128));
    }
    return result;
}

// Aplica fade out
std::vector<int8_t> applyFadeOut(const std::vector<int8_t>& input, float fadeDuration) {
    int fadeSamples = static_cast<int>(SAMPLE_RATE * fadeDuration);
    int numSamples = input.size();
    std::vector<int8_t> output;
    output.reserve(numSamples);
    
    for (int i = 0; i < numSamples; ++i) {
        float fade = 1.0f;
        if (i >= numSamples - fadeSamples) {
            fade = static_cast<float>(numSamples - i) / fadeSamples;
        }
        int val = 128 + static_cast<int>((input[i] + 128) * fade);
        output.push_back(static_cast<int8_t>(std::min(255, std::max(0, val)) - 128));
    }
    return output;
}

// ============================================================================
// GENERADORES DE SONIDOS ESPECÍFICOS
// ============================================================================

std::vector<int8_t> generateGunShot() {
    // Ruido inicial (explosión)
    auto noise = generateNoise(0.08f, 0.8f);
    noise = applyEnvelope(noise, 0.005f, 0.05f, 0.2f, 0.03f);
    
    // Tono grave descendente (punch)
    auto punch = generateSquareWave(150.0f, 0.05f, 0.6f);
    punch = applyEnvelope(punch, 0.005f, 0.04f, 0.1f, 0.01f);
    
    auto samples = mixSamples(noise, punch, 0.7f, 0.3f);
    return applyFadeOut(samples, 0.05f);
}

std::vector<int8_t> generateExplosion() {
    // Ruido inicial fuerte
    auto noise = generateNoise(0.4f, 1.0f);
    noise = applyEnvelope(noise, 0.01f, 0.2f, 0.4f, 0.2f);
    
    // Tonos graves descendentes
    auto bass1 = generateSawtoothWave(80.0f, 0.3f, 0.8f);
    bass1 = applyEnvelope(bass1, 0.01f, 0.15f, 0.3f, 0.15f);
    
    auto bass2 = generateSawtoothWave(50.0f, 0.4f, 0.6f);
    bass2 = applyEnvelope(bass2, 0.02f, 0.2f, 0.2f, 0.2f);
    
    auto samples = mixSamples(noise, bass1, 0.5f, 0.3f);
    samples = mixSamples(samples, bass2, 0.8f, 0.2f);
    return applyFadeOut(samples, 0.3f);
}

std::vector<int8_t> generateZombieHit() {
    auto noise = generateNoise(0.1f, 0.7f);
    noise = applyEnvelope(noise, 0.002f, 0.05f, 0.2f, 0.05f);
    
    auto hit = generateSquareWave(200.0f, 0.08f, 0.5f);
    hit = applyEnvelope(hit, 0.002f, 0.04f, 0.1f, 0.04f);
    
    auto samples = mixSamples(noise, hit, 0.6f, 0.4f);
    return applyFadeOut(samples, 0.05f);
}

std::vector<int8_t> generatePlayerHurt() {
    auto noise = generateNoise(0.15f, 0.6f);
    noise = applyEnvelope(noise, 0.005f, 0.08f, 0.3f, 0.07f);
    
    auto alert = generateSquareWave(300.0f, 0.12f, 0.4f);
    alert = applyEnvelope(alert, 0.005f, 0.06f, 0.2f, 0.06f);
    
    return mixSamples(noise, alert, 0.5f, 0.5f);
}

std::vector<int8_t> generatePickup() {
    std::vector<int8_t> samples;
    int freqs[] = {400, 500, 600, 800};
    
    for (int freq : freqs) {
        auto note = generateSquareWave(static_cast<float>(freq), 0.08f, 0.4f);
        note = applyEnvelope(note, 0.005f, 0.02f, 0.6f, 0.03f);
        samples.insert(samples.end(), note.begin(), note.end());
    }
    return samples;
}

std::vector<int8_t> generateAbilityReady() {
    auto tone1 = generateSquareWave(440.0f, 0.1f, 0.5f);
    tone1 = applyEnvelope(tone1, 0.005f, 0.03f, 0.5f, 0.05f);
    
    auto tone2 = generateSquareWave(550.0f, 0.15f, 0.5f);
    tone2 = applyEnvelope(tone2, 0.005f, 0.05f, 0.5f, 0.05f);
    
    tone1.insert(tone1.end(), tone2.begin(), tone2.end());
    return tone1;
}

std::vector<int8_t> generateAbilityUse() {
    auto noise = generateNoise(0.2f, 0.7f);
    noise = applyEnvelope(noise, 0.01f, 0.1f, 0.4f, 0.1f);
    
    auto power = generateSawtoothWave(200.0f, 0.2f, 0.6f);
    power = applyEnvelope(power, 0.01f, 0.08f, 0.5f, 0.1f);
    
    auto samples = mixSamples(noise, power, 0.4f, 0.6f);
    return applyFadeOut(samples, 0.15f);
}

std::vector<int8_t> generateWaveComplete() {
    std::vector<int8_t> samples;
    // Secuencia de notas victoriosas: C5, E5, G5
    int notes[] = {523, 659, 784};
    float durations[] = {0.15f, 0.15f, 0.3f};
    
    for (int i = 0; i < 3; ++i) {
        auto note = generateSquareWave(static_cast<float>(notes[i]), durations[i], 0.5f);
        note = applyEnvelope(note, 0.01f, 0.05f, 0.6f, 0.1f);
        samples.insert(samples.end(), note.begin(), note.end());
        // Pequeño silencio entre notas
        for (int j = 0; j < SAMPLE_RATE / 20; ++j) {
            samples.push_back(0);
        }
    }
    return samples;
}

std::vector<int8_t> generateGameOver() {
    std::vector<int8_t> samples;
    // Notas descendentes: sol, fa, mi, re
    int notes[] = {400, 350, 300, 250};
    float durations[] = {0.3f, 0.3f, 0.4f, 0.5f};
    
    for (int i = 0; i < 4; ++i) {
        auto note = generateSquareWave(static_cast<float>(notes[i]), durations[i], 0.5f);
        note = applyEnvelope(note, 0.02f, 0.1f, 0.5f, 0.15f);
        samples.insert(samples.end(), note.begin(), note.end());
        // Pequeño silencio entre notas
        for (int j = 0; j < SAMPLE_RATE / 10; ++j) {
            samples.push_back(0);
        }
    }
    return samples;
}

std::vector<int8_t> generateZombieMoan() {
    std::vector<int8_t> samples;
    
    for (int cycle = 0; cycle < 3; ++cycle) {
        // Tono grave con vibrato
        for (int i = 0; i < SAMPLE_RATE / 3; ++i) {
            float t = static_cast<float>(i) / SAMPLE_RATE;
            float freq = 100.0f + std::sin(t * 10.0f * 2.0f * M_PI) * 20.0f;
            float value = std::sin(2.0f * M_PI * freq * t);
            int sample = 128 + static_cast<int>(value * 0.4f * 127.0f);
            samples.push_back(static_cast<int8_t>(std::min(255, std::max(0, sample)) - 128));
        }
        // Silencio entre gemidos
        for (int j = 0; j < SAMPLE_RATE / 5; ++j) {
            samples.push_back(0);
        }
    }
    return samples;
}

std::vector<int8_t> generateMenuSelect() {
    auto noise = generateNoise(0.03f, 0.6f);
    return applyEnvelope(noise, 0.001f, 0.02f, 0.1f, 0.01f);
}

// ============================================================================
// FUNCIÓN PARA GUARDAR WAV
// ============================================================================

void saveWav(const std::string& filename, const std::vector<int8_t>& samples) {
    // Calcular tamaños
    int numSamples = samples.size();
    int dataSize = numSamples * sizeof(int8_t);
    int byteRate = SAMPLE_RATE * NUM_CHANNELS * (BIT_DEPTH / 8);
    int blockAlign = NUM_CHANNELS * (BIT_DEPTH / 8);
    int fileSize = 36 + dataSize;
    
    // Crear header
    RIFFHeader riff;
    riff.fileSize = fileSize;
    
    FMTChunk fmt;
    fmt.byteRate = byteRate;
    fmt.blockAlign = blockAlign;
    
    DATAChunk data;
    data.dataSize = dataSize;
    
    // Escribir archivo
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear " << filename << std::endl;
        return;
    }
    
    file.write(reinterpret_cast<const char*>(&riff), sizeof(RIFFHeader));
    file.write(reinterpret_cast<const char*>(&fmt), sizeof(FMTChunk));
    file.write(reinterpret_cast<const char*>(&data), sizeof(DATAChunk));
    file.write(reinterpret_cast<const char*>(samples.data()), dataSize);
    file.close();
    
    std::cout << "  [OK] " << filename << " (" << dataSize << " bytes)" << std::endl;
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    std::cout << "============================================================" << std::endl;
    std::cout << "   GENERADOR DE AUDIO 8-BIT - Z-WARE (C++ PURO)" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << std::endl;
    
    // Crear directorio de sonidos
    system("mkdir -p assets/sounds");
    
    std::cout << "[Generando sonidos de armas...]" << std::endl;
    saveWav("assets/sounds/disparo.wav", generateGunShot());
    saveWav("assets/sounds/explosion.wav", generateExplosion());
    saveWav("assets/sounds/zombie_hit.wav", generateZombieHit());
    
    std::cout << std::endl << "[Generando sonidos del jugador...]" << std::endl;
    saveWav("assets/sounds/jugador_dano.wav", generatePlayerHurt());
    saveWav("assets/sounds/pickup.wav", generatePickup());
    
    std::cout << std::endl << "[Generando sonidos de habilidades...]" << std::endl;
    saveWav("assets/sounds/habilidad_lista.wav", generateAbilityReady());
    saveWav("assets/sounds/habilidad_uso.wav", generateAbilityUse());
    
    std::cout << std::endl << "[Generando sonidos de juego...]" << std::endl;
    saveWav("assets/sounds/oleada_completa.wav", generateWaveComplete());
    saveWav("assets/sounds/game_over.wav", generateGameOver());
    
    std::cout << std::endl << "[Generando sonidos de zombies...]" << std::endl;
    saveWav("assets/sounds/zombie_gemido.wav", generateZombieMoan());
    
    std::cout << std::endl << "[Generando sonidos de menu...]" << std::endl;
    saveWav("assets/sounds/menu_seleccion.wav", generateMenuSelect());
    
    std::cout << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "   AUDIO GENERADO EXITOSAMENTE!" << std::endl;
    std::cout << "============================================================" << std::endl;
    
    return 0;
}