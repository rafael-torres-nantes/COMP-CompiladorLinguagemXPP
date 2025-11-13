# Script de Testes - Compilador X++

Write-Host "========================================"
Write-Host "  TESTES DO COMPILADOR X++"
Write-Host "========================================"
Write-Host ""

$tests = @(
    # Testes de Sucesso
    @{Name="Programa valido simples"; File="test_success.xpp"; Expected="success"},
    @{Name="Programa valido complexo"; File="test_valid.xpp"; Expected="success"},
    @{Name="Programa completo"; File="test_completo.xpp"; Expected="success"},
    @{Name="Teste semantico completo"; File="test_semantico.xpp"; Expected="success"},
    @{Name="Teste simples"; File="test_simple.xpp"; Expected="success"},
    
    # Testes de Erro Lexico
    @{Name="Erro lexico - caractere invalido"; File="test_erro_lexico.xpp"; Expected="error"},
    
    # Testes de Erro Sintatico
    @{Name="Erro sintatico"; File="test_erro_sintaxe.xpp"; Expected="error"},
    
    # Testes de Erro Semantico
    @{Name="Erro semantico - variavel nao declarada"; File="test_erro_semantico1.xpp"; Expected="error"},
    @{Name="Erro semantico - redeclaracao variavel"; File="test_erro_semantico2.xpp"; Expected="error"},
    @{Name="Erro semantico - classe nao declarada"; File="test_erro_semantico3.xpp"; Expected="error"},
    @{Name="Erro semantico - redeclaracao classe"; File="test_erro_semantico4.xpp"; Expected="error"},
    @{Name="Erro semantico - heranca invalida"; File="test_erro_semantico5.xpp"; Expected="error"}
)

$passed = 0
$failed = 0
$testNumber = 0

Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host "TESTES DE SUCESSO" -ForegroundColor Cyan
Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host ""

foreach ($test in $tests) {
    $testNumber++
    
    # Separador visual para diferentes categorias
    if ($test.Name -match "^Erro lexico" -and $testNumber -eq 6) {
        Write-Host ""
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host "TESTES DE ERRO LEXICO" -ForegroundColor Cyan
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host ""
    }
    elseif ($test.Name -match "^Erro sintatico" -and $testNumber -eq 7) {
        Write-Host ""
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host "TESTES DE ERRO SINTATICO" -ForegroundColor Cyan
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host ""
    }
    elseif ($test.Name -match "^Erro semantico" -and $testNumber -eq 8) {
        Write-Host ""
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host "TESTES DE ERRO SEMANTICO" -ForegroundColor Cyan
        Write-Host "----------------------------------------" -ForegroundColor Cyan
        Write-Host ""
    }
    
    Write-Host "[$testNumber/$($tests.Count)] $($test.Name)" -ForegroundColor Yellow
    Write-Host "  Arquivo: $($test.File)"
    
    $output = & .\xpp_compiler.exe "tests\$($test.File)" 2>&1
    $exitCode = $LASTEXITCODE
    
    if ($test.Expected -eq "success") {
        if ($exitCode -eq 0) {
            Write-Host "  PASSOU" -ForegroundColor Green
            $passed++
        } else {
            Write-Host "  FALHOU" -ForegroundColor Red
            $failed++
        }
    } else {
        if ($exitCode -ne 0) {
            Write-Host "  PASSOU" -ForegroundColor Green
            $passed++
        } else {
            Write-Host "  FALHOU" -ForegroundColor Red
            $failed++
        }
    }
    Write-Host ""
}

Write-Host ""
Write-Host "========================================"
Write-Host "RESUMO DOS TESTES" -ForegroundColor White
Write-Host "========================================"
Write-Host "Total de testes:  $($tests.Count)"
Write-Host "Testes passados:  $passed" -ForegroundColor Green
Write-Host "Testes falhados:  $failed" -ForegroundColor Red
Write-Host ""

if ($failed -eq 0) {
    Write-Host "========================================"
    Write-Host "  TODOS OS TESTES PASSARAM!" -ForegroundColor Green
    Write-Host "========================================"
    exit 0
} else {
    Write-Host "========================================"
    Write-Host "  $failed TESTE(S) FALHARAM!" -ForegroundColor Red
    Write-Host "========================================"
    exit 1
}
